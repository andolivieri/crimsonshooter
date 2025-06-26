#include "grenadeprojectile.h"
#include "game.h"
#include "damagemodel.h"
#include "collidercomponent.h"
#include "spritecomponent.h"
#include "timercomponent.h"
#include "shockwavecomponent.h"
#include "soundcomponent.h"
#include "math2d.h"
#include <cmath>
#include <SDL.h>
#include <bloodspit.h>

GrenadeProjectileComponent::GrenadeProjectileComponent(const Vector2D& startPos, const Vector2D& targetPos)
    : startPosition(startPos), targetPosition(targetPos)
{
    calculateMovement();
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
    
    // Update position using simple linear movement
    transform->pos.x += velocity.x * deltaTime;
    transform->pos.y += velocity.y * deltaTime;
    
    // Check if grenade has reached target or hit ground
    float distanceToTarget = Math2D::distanceBetweenPoints(transform->pos, targetPosition);
    
    if (timeAlive >= flightTime || distanceToTarget < 1.0f) {
        explode();
    }
}

void GrenadeProjectileComponent::calculateMovement()
{
    // Calculate simple linear movement from start to target
    Vector2D displacement = targetPosition - startPosition;
    float totalDistance = displacement.magnitude();
    
    // Calculate flight time based on constant speed
    flightTime = totalDistance / speed;
    
    // Calculate velocity components for linear movement
    if (flightTime > 0) {
        velocity.x = displacement.x / flightTime;
        velocity.y = displacement.y / flightTime;
    } else {
        velocity.x = 0;
        velocity.y = 0;
    }
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
    explosion.addComponent<TransformComponent>(transform->pos.x - 32, transform->pos.y - 32, 128, 128);
    explosion.addComponent<SpriteComponent>("assets/explosion.png")
        .setSrcRect({0, 0, 64, 64})
        .addAnimation("explode", {0, 0, 16, 50}, ANIMATION_FLAG_HIDE_AFTER_END)
        .play("explode", 1);
    explosion.addComponent<SoundComponent>().play("assets/explosion.wav", 0, 2);
    explosion.addComponent<DecayComponent>(2000);
    explosion.addGroup(groupProjectiles);
    
    // Create shockwave visual effect - match damage radius
    const float maxDamageRadius = 160.0f;
    auto& shockwave = entity->m_manager.addEntity();
    shockwave.addComponent<TransformComponent>(
        transform->pos.x - maxDamageRadius, 
        transform->pos.y - maxDamageRadius, 
        maxDamageRadius * 2, 
        maxDamageRadius * 2
    );
    shockwave.addComponent<ShockwaveComponent>(maxDamageRadius, 1 , 0.5f);
    shockwave.addComponent<DecayComponent>(2000);
    shockwave.addGroup(groupProjectiles);
    
    // Create explosion damage area - match shockwave max radius (160 pixels)
    auto& damageArea = entity->m_manager.addEntity();
    damageArea.addComponent<TransformComponent>(
        transform->pos.x - maxDamageRadius, 
        transform->pos.y - maxDamageRadius, 
        maxDamageRadius * 2, 
        maxDamageRadius * 2
    );

    Vector2D explosionCenter = transform->pos;
    damageArea.addComponent<ColliderComponent>().onCollision([explosionCenter, maxDamageRadius](Entity& target) {
        if (target.hasComponent<DamageModelComponent>() && target.hasGroup(groupEnemies)) {
            auto& dm = target.getComponent<DamageModelComponent>();
            
            if (target.hasComponent<TransformComponent>()) {
                auto& targetTransform = target.getComponent<TransformComponent>();
                
                // Calculate distance from explosion center to target center
                Vector2D targetCenter;
                targetCenter.x = targetTransform.pos.x + targetTransform.width / 2.0f;
                targetCenter.y = targetTransform.pos.y + targetTransform.height / 2.0f;
                
                Vector2D explosionCenterAdjusted;
                explosionCenterAdjusted.x = explosionCenter.x + 16; // Assuming grenade is 32x32, center it
                explosionCenterAdjusted.y = explosionCenter.y + 16;
                
                float distance = Math2D::distanceBetweenPoints(targetCenter, explosionCenterAdjusted);
                
                // Only damage if within explosion radius
                if (distance <= maxDamageRadius) {
                    // Calculate damage falloff (inversely proportional to distance)
                    float maxDamage = 100.0f;
                    float minDamage = 20.0f;
                    float damageMultiplier = 1.0f - (distance / maxDamageRadius);
                    float finalDamage = minDamage + (maxDamage - minDamage) * damageMultiplier;
                    
                    dm.health -= static_cast<int>(finalDamage);
                    createBloodSpit(target);
                    
                    // Calculate knockback (inversely proportional to distance)
                    Vector2D knockbackDir;
                    knockbackDir.x = targetCenter.x - explosionCenterAdjusted.x;
                    knockbackDir.y = targetCenter.y - explosionCenterAdjusted.y;
                    
                    // Normalize knockback direction
                    float dirLength = knockbackDir.magnitude();
                    if (dirLength > 0) {
                        knockbackDir.x /= dirLength;
                        knockbackDir.y /= dirLength;
                    }
                    
                    // Calculate knockback strength (stronger at close range)
                    float maxKnockback = 25.0f;
                    float minKnockback = 3.0f;
                    float knockbackMultiplier = 1.0f - (distance / maxDamageRadius);
                    float knockbackStrength = minKnockback + (maxKnockback - minKnockback) * knockbackMultiplier;
                    
                    targetTransform.velocity.x = knockbackDir.x * knockbackStrength;
                    targetTransform.velocity.y = knockbackDir.y * knockbackStrength;
                }
            }
        }
    });
    
    // Damage area exists for a short time
    damageArea.addComponent<DecayComponent>(100);
    damageArea.addGroup(groupProjectiles);
}