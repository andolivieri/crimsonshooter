#ifndef GRENADEPROJECTILE_H
#define GRENADEPROJECTILE_H

#include "ecs.h"
#include "ecs/core/transformcomponent.h"
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
    Vector2D velocity;
    
    float speed = 600.0f; // pixels/second
    float timeAlive = 0.0f;
    float flightTime = 0.0f; // Total time to reach target
    uint32_t lastUpdateTime = 0;
    
    bool hasExploded = false;
    
    // Rotation properties
    float rotationSpeed = 720.0f; // degrees per second
    float currentRotation = 0.0f;
    
    void calculateMovement();
    void explode();
    void createExplosion();
};

#endif // GRENADEPROJECTILE_H