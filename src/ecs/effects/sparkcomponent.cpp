#include "sparkcomponent.h"
#include "engine/game.h"
#include "engine/texturemanager.h"
#include <cstdlib>
#include <cmath>

SparkComponent::SparkComponent(Vector2D worldPos, Config cfg)
    : m_origin(worldPos), m_color(cfg.color), m_count(cfg.count),
      m_lifeMsec(static_cast<uint32_t>(cfg.sparkDuration > 1.f ? cfg.sparkDuration : 1.f))
{
}

void SparkComponent::init()
{
    m_startTicks = SDL_GetTicks();

    m_particles.reserve(m_count);
    for (int i = 0; i < m_count; i++) {
        float ang = (rand() % 360) * 3.14159265f / 180.f;
        float spd = 0.6f + (rand() % 100) / 100.f * 1.8f;
        Particle p;
        p.pos = m_origin;
        p.vel = Vector2D(static_cast<float>(std::cos(ang)) * spd,
                         static_cast<float>(std::sin(ang)) * spd);
        m_particles.push_back(p);
    }

    entity->addGroup(groupProjectiles);
}

void SparkComponent::update()
{
    uint32_t age = SDL_GetTicks() - m_startTicks;
    if (age >= m_lifeMsec) {
        entity->destroy();
        return;
    }

    for (auto& p : m_particles) {
        p.pos.x += p.vel.x;
        p.pos.y += p.vel.y;
        // friction so the puff settles instead of flying off
        p.vel.x *= 0.90f;
        p.vel.y *= 0.90f;
    }
}

void SparkComponent::draw()
{
    uint32_t age = SDL_GetTicks() - m_startTicks;
    float t = static_cast<float>(age) / static_cast<float>(m_lifeMsec);
    if (t > 1.f) return;

    Uint8 a = static_cast<Uint8>((1.f - t) * m_color.a);
    SDL_SetRenderDrawBlendMode(TextureManager::renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(TextureManager::renderer, m_color.r, m_color.g, m_color.b, a);

    for (const auto& p : m_particles) {
        SDL_Rect r{
            static_cast<int>(p.pos.x - Game::camera.x),
            static_cast<int>(p.pos.y - Game::camera.y),
            2, 2
        };
        SDL_RenderFillRect(TextureManager::renderer, &r);
    }

    SDL_SetRenderDrawBlendMode(TextureManager::renderer, SDL_BLENDMODE_NONE);
}
