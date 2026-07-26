#ifndef TURRETCOMPONENT_H
#define TURRETCOMPONENT_H

#include <string>
#include <cstdint>
#include "ecs/ecs.h"
#include "helpers/vector2d.h"

class WeaponBayComponent;
class TransformComponent;

// temporary auto-firing turret.
class TurretComponent : public Component
{
public:
    struct Config {
        std::string weaponId   = "mg";               // uzi | shotgun | mg
        std::string baseSprite = "assets/turret.png"; // turret body
        float duration    = 10.0f;                   // seconds alive
        float engageRange  = 800.f;                  // only fire at enemies within this
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
};

#endif // TURRETCOMPONENT_H
