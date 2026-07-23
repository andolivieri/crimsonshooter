#include "straferuncomponent.h"
#include "sparkcomponent.h"
#include "trailcomponent.h"
#include "ecs/weapons/projectilecomponent.h"
#include "engine/game.h"
#include "engine/gamemap.h"
#include "engine/texturemanager.h"
#include "helpers/math2d.h"
#include "helpers/utils.h"
#include "SDL_mixer.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

// todo andoli: swap these placeholders for dedicated A10 art & audio
static constexpr const char* SPR_A10   = "assets/bomber.png";
static constexpr const char* SFX_BRRRT = "assets/sounds/brrrrt.ogg";
static constexpr const char* SFX_ROAR  = "assets/sounds/jet_flyby.ogg";

static constexpr int BRRRT_CHANNEL = 6;
static constexpr int ROAR_CHANNEL  = 7;

static const SDL_Color BULLET_COLOR = {225, 210, 120, 255};
static constexpr int BULLET_LEN = 8;

// Clip the ray p + t*d against the axis-aligned map rect [0,W]x[0,H].
// Returns the parameter range [tmin,tmax] of the segment inside the rect.
static bool clipRayToRect(const Vector2D& p, const Vector2D& d,
                          float W, float H, float& tmin, float& tmax)
{
    float t0 = -1e9f, t1 = 1e9f;
    const float eps = 1e-6f;

    auto clip = [&](float pp, float qq) -> bool {
        if (std::fabs(pp) < eps) return qq >= 0.f; // parallel: inside iff qq>=0
        float r = qq / pp;
        if (pp < 0.f) { if (r > t1) return false; if (r > t0) t0 = r; }
        else          { if (r < t0) return false; if (r < t1) t1 = r; }
        return true;
    };

    if (!clip(-d.x, p.x))     return false;
    if (!clip( d.x, W - p.x)) return false;
    if (!clip(-d.y, p.y))     return false;
    if (!clip( d.y, H - p.y)) return false;

    if (t1 < t0) return false;
    tmin = t0;
    tmax = t1;
    return true;
}

StrafeRunComponent::StrafeRunComponent(Vector2D callerWorldPos, Config cfg)
    : m_cfg(cfg), m_caller(callerWorldPos)
{
}

void StrafeRunComponent::init()
{
    m_startTicks = SDL_GetTicks();
    planStrafe();
}

void StrafeRunComponent::planStrafe()
{
    const float W = static_cast<float>(GameMap::mapWidth);
    const float H = static_cast<float>(GameMap::mapHeight);

    // Point near the caller, pulled away from the borders (more central).
    const float mx = W * m_cfg.borderMargin;
    const float my = H * m_cfg.borderMargin;
    Vector2D A = m_caller;
    A.x = std::min(std::max(A.x, mx), W - mx);
    A.y = std::min(std::max(A.y, my), H - my);
    A.x += static_cast<float>((rand() % 81) - 40);
    A.y += static_cast<float>((rand() % 81) - 40);

    // A random point on one of the four map borders.
    Vector2D B;
    switch (rand() % 4) {
        case 0:  B = Vector2D(static_cast<float>(rand() % std::max(1, (int)W)), 0.f); break; // top
        case 1:  B = Vector2D(static_cast<float>(rand() % std::max(1, (int)W)), H);   break; // bottom
        case 2:  B = Vector2D(0.f, static_cast<float>(rand() % std::max(1, (int)H))); break; // left
        default: B = Vector2D(W, static_cast<float>(rand() % std::max(1, (int)H)));    break; // right
    }

    // Unit heading A -> B; the whole run follows this direction.
    float dx = B.x - A.x, dy = B.y - A.y;
    float len = std::sqrt(dx * dx + dy * dy);
    if (len < 1.f) { dx = 1.f; dy = 0.f; len = 1.f; }
    m_dir  = Vector2D(dx / len, dy / len);
    m_perp = Vector2D(-m_dir.y, m_dir.x);

    // Build the parallel lanes, each clipped border-to-border.
    m_lanes.clear();
    const float half = (m_cfg.lanes - 1) / 2.0f;
    for (int i = 0; i < m_cfg.lanes; i++) {
        float off = (i - half) * m_cfg.laneSpacing;
        Vector2D anchor(A.x + m_perp.x * off, A.y + m_perp.y * off);

        float tmin, tmax;
        if (!clipRayToRect(anchor, m_dir, W, H, tmin, tmax))
            continue;

        Lane lane;
        lane.entry = Vector2D(anchor.x + m_dir.x * tmin, anchor.y + m_dir.y * tmin);
        lane.exit  = Vector2D(anchor.x + m_dir.x * tmax, anchor.y + m_dir.y * tmax);
        m_lanes.push_back(lane);
    }
}

void StrafeRunComponent::update()
{
    float elapsed = (SDL_GetTicks() - m_startTicks) / 1000.f;

    // Kick off the "brrrrt" and start firing after the intro delay.
    if (!m_brrrtStarted && elapsed >= m_cfg.startDelay) {
        auto& snd = entity->m_manager.addEntity();
        snd.addComponent<SoundComponent>().play(SFX_BRRRT, 0, BRRRT_CHANNEL);
        snd.addComponent<DecayComponent>(6 * ONE_SECOND);
        m_brrrtStarted = true;
        m_firing = true;
    }

    if (m_firing) {
        float frontProgress = (elapsed - m_cfg.startDelay) / m_cfg.fireDuration;
        if (frontProgress >= 1.0f) {
            m_firing = false;
            spawnPlaneShadow();
        } else {
            spawnBulletVolley(frontProgress);
        }
    }

    // The bullets are self-managing projectile entities now, so once the guns
    // fall silent and the plane has flown over there is nothing left to do.
    if (!m_firing && m_planeSpawned &&
        elapsed > m_cfg.startDelay + m_cfg.fireDuration + 0.5f) {
        entity->destroy();
    }
}

void StrafeRunComponent::spawnBulletVolley(float frontProgress)
{
    // Captured by value so the range-end spark outlives this component safely.
    EntityManager* mgr = &entity->m_manager;
    SparkComponent::Config sparkCfg;
    sparkCfg.sparkDuration = m_cfg.sparkDuration;
    sparkCfg.color = {157, 116, 86, 255};

    for (const auto& lane : m_lanes) {
        float ex = lane.exit.x - lane.entry.x;
        float ey = lane.exit.y - lane.entry.y;

        for (int b = 0; b < m_cfg.bulletsPerLanePerFrame; b++) {
            // impact walks along the lane with a touch of back-scatter
            float along = frontProgress - (rand() % 100) / 100.f * 0.05f;
            if (along < 0.f) along = 0.f;

            float fx = lane.entry.x + ex * along;
            float fy = lane.entry.y + ey * along;
            float ps = ((rand() % 1000) / 1000.f - 0.5f) * m_cfg.impactScatter;
            Vector2D impact(fx + m_perp.x * ps, fy + m_perp.y * ps);

            // Fire from behind the impact along the run heading; the bullet's
            // range is the travel distance, so it "stops" right at the ground.
            Vector2D src(impact.x - m_dir.x * m_cfg.bulletTravel,
                         impact.y - m_dir.y * m_cfg.bulletTravel);

            auto& bullet = mgr->addEntity();
            auto& pc = bullet.addComponent<ProjectileComponent>(src, impact)
                             .setDamage(m_cfg.bulletDamage)
                             .setRange(static_cast<int>(m_cfg.bulletTravel))
                             .setSpeed(m_cfg.bulletSpeed)
                             .setColor(BULLET_COLOR)
                             .onRangeEnd([mgr, sparkCfg](const Vector2D& p) {
                                 mgr->addEntity().addComponent<SparkComponent>(p, sparkCfg);
                             });
            pc.renderLength = BULLET_LEN;
            pc.renderThickness = 2;
            TrailComponent::Config tracerCfg;
            tracerCfg.color = {126,126,126,255};
            tracerCfg.maxPoints = 100;
            tracerCfg.thickness = 2;
            bullet.addComponent<TrailComponent>(tracerCfg);
        }
    }
}

void StrafeRunComponent::spawnPlaneShadow()
{
    if (m_planeSpawned || m_lanes.empty()) return;
    m_planeSpawned = true;

    const Lane& center = m_lanes[m_lanes.size() / 2];

    const float speed = 16.f;
    Vector2D spriteSize(256, 138);
    const int spriteScale = 3;
    float angle = static_cast<float>(Math2D::angleBetweenPoints(center.entry, center.exit));

    // start well outside the entry border so it flies fully across the field
    Vector2D start(center.entry.x - m_dir.x * 400.f,
                   center.entry.y - m_dir.y * 400.f);

    auto& plane = entity->m_manager.addEntity();
    auto& tr = plane.addComponent<TransformComponent>(
        static_cast<int>(start.x), static_cast<int>(start.y),
        static_cast<int>(spriteSize.x * spriteScale),
        static_cast<int>(spriteSize.y * spriteScale));
    plane.addComponent<SpriteComponent>(SPR_A10)
        .setSrcRect({0, 0, static_cast<int>(spriteSize.x), static_cast<int>(spriteSize.y)})
        .setAbsolute(false)
        .setColorMod(35, 35, 40) // darken into a shadow
        .setAlpha(120);
    tr.velocity = Vector2D(m_dir.x * speed, m_dir.y * speed);
    tr.rotation = angle + 90;
    plane.addComponent<DecayComponent>(10 * ONE_SECOND, 8 * ONE_SECOND);
    plane.addGroup(groupOverlayBg);

    auto& snd = entity->m_manager.addEntity();
    snd.addComponent<SoundComponent>().play(SFX_ROAR, 0, ROAR_CHANNEL);
    snd.addComponent<DecayComponent>(6 * ONE_SECOND);
}
