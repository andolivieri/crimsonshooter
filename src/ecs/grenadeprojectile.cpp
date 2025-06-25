#include "grenadeprojectile.h"
#include "game.h"
#include "damagemodel.h"
#include "collidercomponent.h"
#include "spritecomponent.h"
#include "timercomponent.h"
#include "math2d.h"
#include <cmath>
#include <SDL.h>

GrenadeProjectileComponent::GrenadeProjectileComponent(const Vector2D& startPos, const Vector2D& targetPos)
    : startPosition(startPos), targetPosition(targetPos)
{
    calculateInitialVelocity();
    lastUpdateTime = SDL_GetTicks();
}

void GrenadeProjectileComponent::init()
{
    transform = &entity->getComponent<TransformComponent>();
    transform->pos = startPosition;
}

void GrenadeProjectileComponent::update()
{
    if (hasExploded || !transform) return;
    
    uint32_t currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastUpdateTime) / 1000.0f; // Convert to seconds
    lastUpdateTime = currentTime;
    timeAlive += deltaTime;
    
    // Update position using physics
    currentVelocity.y += gravity * deltaTime; // Apply gravity
    
    transform->pos.x += currentVelocity.x * deltaTime;
    transform->pos.y += currentVelocity.y * deltaTime;
    
    // Check if grenade has reached target or hit ground
    float distanceToTarget = Math2D::distanceBetweenPoints(transform->pos, targetPosition);
    
    if (timeAlive >= flightTime || distanceToTarget < 10.0f) {
        explode();
    }
}

void GrenadeProjectileComponent::calculateInitialVelocity()
{
    // Calculate the trajectory for an arc
    Vector2D displacement = targetPosition - startPosition;
    float horizontalDistance = displacement.magnitude();
    
    // Calculate flight time based on arc trajectory
    flightTime = horizontalDistance / initialSpeed;
    
    // Calculate initial velocity components
    currentVelocity.x = displacement.x / flightTime;
    
    // Calculate initial Y velocity to create an arc
    // Using kinematic equation: y = y0 + v0*t + 0.5*g*t^2
    // We want the grenade to reach the target at ground level
    float initialYVelocity = (displacement.y - 0.5f * gravity * flightTime * flightTime) / flightTime;
    
    // Add extra height for the arc
    initialYVelocity -= arcHeight / flightTime;
    
    currentVelocity.y = initialYVelocity;
}

void GrenadeProjectileComponent::explode()
{
    if (hasExploded) return;
    
    hasExploded = true;
    createExplosion();
    
    // Remove the grenade entity
    entity->destroy();
}

void GrenadeProjectileComponent::createExplosion()
{
    if (!transform) return;
    
    std::cout << "Grenade exploded at position: " << transform->pos.x << ", " << transform->pos.y << std::endl;
    // Create explosion visual effect
    auto& explosion = entity->m_manager.addEntity();
    explosion.addComponent<TransformComponent>(transform->pos.x - 32, transform->pos.y - 32, 64, 64);
    explosion.addComponent<SpriteComponent>("assets/explosion.png")
        .setSrcRect({0, 0, 256, 256})
        .addAnimation("explode", {0, 0, 8, 50}, ANIMATION_FLAG_HIDE_AFTER_END)
        .play("explode", 1);
    explosion.addComponent<DecayComponent>(2000);
    explosion.addGroup(groupProjectiles);
    
    // Create explosion damage area
    auto& damageArea = entity->m_manager.addEntity();
    damageArea.addComponent<TransformComponent>(transform->pos.x - 50, transform->pos.y - 50, 100, 100);

    auto tTransform = transform;
    damageArea.addComponent<ColliderComponent>().onCollision([this, tTransform](Entity& target) {
        if (target.hasComponent<DamageModelComponent>() && target.hasGroup(groupEnemies)) {
            auto& dm = target.getComponent<DamageModelComponent>();
            dm.health -= 75; // High damage for explosionì
            
            // Add knockback effect
            if (target.hasComponent<TransformComponent>()) {
                auto& targetTransform = target.getComponent<TransformComponent>();
                Vector2D knockbackDir;
                knockbackDir.x = targetTransform.pos.x - tTransform->pos.x;
                knockbackDir.y = targetTransform.pos.y - tTransform->pos.y;
                
                // Normalize knockback direction
                auto length = knockbackDir.magnitude();
                if (length > 0) {
                    knockbackDir.x /= length;
                    knockbackDir.y /= length;
                }
                
                targetTransform.velocity.x = knockbackDir.x * 10.0f;
                targetTransform.velocity.y = knockbackDir.y * 10.0f;
            }
        }
    });
    
    // Damage area exists for a short time
    damageArea.addComponent<DecayComponent>(100);
    
    damageArea.addGroup(groupProjectiles);
}