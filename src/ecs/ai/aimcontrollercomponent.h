#ifndef AIMCONTROLLERCOMPONENT_H
#define AIMCONTROLLERCOMPONENT_H

#include <string>
#include "ecs/ecs.h"

class TransformComponent;

// Picks a target and turns the entity toward it at a limited rate.
// (Sensing and aiming only)
class AimControllerComponent : public Component
{
public:
    struct Config {
        Config(); // targetGroup defaults to groupEnemies

        float rotationSpeed  = 120.f; // deg/sec, <= 0 snaps instantly
        float engageRange    = 800.f; // acquisition radius
        float disengageRange = 0.f;   // 0 = hold the target until it dies
        float aimErrorDeg    = 0.f;   // constant aim bias, for imperfect shooters
        Group targetGroup;
    };

    explicit AimControllerComponent(Config cfg) : m_cfg(cfg) {}
    AimControllerComponent() : AimControllerComponent(Config{}) {}

    void init() override;
    void update() override;

    bool hasTarget() const { return target() != nullptr; }
    Entity* target() const;                        // resolved from the tag, may be null
    const std::string& targetTag() const { return m_targetTag; }
    double aimError() const { return m_aimError; } // signed degrees left to turn
    void forgetTarget() { m_targetTag.clear(); }

    Config& config() { return m_cfg; }

private:
    bool targetStillValid() const;
    std::string acquireTarget() const; // tag of the nearest live candidate, "" if none

    Config m_cfg;
    TransformComponent* m_transform = nullptr;
    std::string m_targetTag; // the target is held by tag only, never by pointer
    double m_aimError = 0;
};

#endif // AIMCONTROLLERCOMPONENT_H
