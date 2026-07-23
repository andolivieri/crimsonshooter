#ifndef STRAFERUNCOMPONENT_H
#define STRAFERUNCOMPONENT_H

#include <SDL.h>
#include <vector>
#include "ecs/ecs.h"
#include "helpers/vector2d.h"

// brrrrt
class StrafeRunComponent : public Component
{
public:
    // All the knobs live here so the run is easy to tune.
    struct Config {
        int   lanes = 4;                  // parallel bullet lines (3-4)
        float laneSpacing = 26.f;         // px between adjacent lanes
        float startDelay = 0.35f;         // s before the guns open up
        float fireDuration = 2.3f;        // s the rain sweeps across the field
        float bulletSpeed = 46.f;         // px/frame along the strafe line
        float bulletTravel = 400.f;       // range each bullet flies before it stops & sparks
        int   bulletsPerLanePerFrame = 1; // rain density
        float impactScatter = 22.f;       // perpendicular spread within a lane
        int   bulletDamage = 60;          // dealt to enemies the bullet passes through
        float borderMargin = 0.18f;       // keep the caller point this far (fraction) from borders
        float sparkDuration = 300;       // spark particle duration
    };

    StrafeRunComponent(Vector2D callerWorldPos, Config cfg = {});

    void init() override;
    void update() override;

private:
    struct Lane {
        Vector2D entry;
        Vector2D exit;
    };

    Config   m_cfg;
    Vector2D m_caller;
    Vector2D m_dir;   // unit strafe heading
    Vector2D m_perp;  // unit perpendicular to the heading

    std::vector<Lane>   m_lanes;

    uint32_t m_startTicks = 0;
    bool m_firing = false;
    bool m_brrrtStarted = false;
    bool m_planeSpawned = false;

    void planStrafe();
    void spawnBulletVolley(float frontProgress);
    void spawnPlaneShadow();
};

#endif // STRAFERUNCOMPONENT_H
