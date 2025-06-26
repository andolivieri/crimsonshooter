#ifndef GRENADECOMPONENT_H
#define GRENADECOMPONENT_H

#include "ecs.h"
#include "transformcomponent.h"
#include "inputcomponent.h"
#include "vector2d.h"

class GrenadeComponent : public Component
{
public:
    GrenadeComponent() = default;
    
    void init() override;
    void update() override;
    void draw() override;
    
    void throwGrenade(float chargeLevel);
    
private:
    InputComponent* input = nullptr;
    TransformComponent* transform = nullptr;
    
    bool isCharging = false;
    float chargeLevel = 0.0f;
    float maxChargeLevel = 1.0f;
    float chargeSpeed = 2.0f; // charge per second
    
    uint32_t chargeStartTime = 0;
    
    void drawChargeBar();
    void drawTrajectoryLine();
    Vector2D calculateLandingPoint(float chargeLevel);
};

#endif // GRENADECOMPONENT_H