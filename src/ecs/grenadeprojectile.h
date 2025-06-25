#ifndef GRENADEPROJECTILE_H
#define GRENADEPROJECTILE_H

#include "ecs.h"
#include "transformcomponent.h"
#include "vector2d.h"

class GrenadeProjectileComponent : public Component
{
public:
    GrenadeProjectileComponent(const Vector2D& startPos, const Vector2D& targetPos);
    
    void init() override;
    void update() override;
    
private:
    TransformComponent* transform = nullptr;
    
    Vector2D startPosition;
    Vector2D targetPosition;
    Vector2D currentVelocity;
    
    float gravity = 300.0f; // pixels/second^2
    float initialSpeed = 400.0f; // pixels/second
    float arcHeight = 100.0f; // pixels
    
    float timeAlive = 0.0f;
    float flightTime = 0.0f; // Total time to reach target
    uint32_t lastUpdateTime = 0;
    
    bool hasExploded = false;
    
    void calculateInitialVelocity();
    void explode();
    void createExplosion();
};

#endif // GRENADEPROJECTILE_H