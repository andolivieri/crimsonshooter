#ifndef TURRETCOMPONENT_H
#define TURRETCOMPONENT_H

#include <string>
#include <cstdint>
#include "ecs/ecs.h"
#include "helpers/vector2d.h"

class WeaponBayComponent;
class TransformComponent;
class AimControllerComponent;

// temporary auto-firing turret.
class TurretComponent : public Component
{
public:
    struct Config {
        std::string weaponId   = "mg";               // uzi | shotgun | mg
        std::string baseSprite = "assets/turret.png"; // turret body
        float duration    = 10.0f;                   // seconds alive
        float engageRange  = 800.f;                  // only acquire enemies within this
        float rotationSpeed = 360.f;                 // deg/sec, <= 0 snaps instantly
        float disengageRange = 0.f;                  // 0 = stay on a target until it dies
        Vector2D attachPoint = {0, 0};               // weapon offset on the body

        int overrideRate     = 0;
        int overrideDamage   = 0;
        int overrideRange    = 0;
        int overrideMagazine = 0;
    };

    TurretComponent(Config cfg) : m_cfg(cfg) {}
    TurretComponent() : TurretComponent(Config{}) {}

    void init() override;
    void update() override;

private:
    Config m_cfg;
    TransformComponent* m_transform = nullptr;
    TransformComponent* m_bodyTransform = nullptr;
    WeaponBayComponent* m_bay = nullptr;
    AimControllerComponent* m_aim = nullptr;
};

#endif // TURRETCOMPONENT_H
