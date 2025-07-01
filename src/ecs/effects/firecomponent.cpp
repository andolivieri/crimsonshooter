#include "firecomponent.h"
#include "ecs/components.h"
#include "game.h"
#include <iostream>

FireComponent::FireComponent(float duration, int damagePerSecond)
    : duration(duration), damagePerSecond(damagePerSecond)
{
}

void FireComponent::init()
{
    if (!entity->hasComponent<TransformComponent>())
    {
        return;
    }

    target_transform = &entity->getComponent<TransformComponent>();
    startTime = SDL_GetTicks();
    lastDamageTime = startTime;

    createFireVisual();
}

void FireComponent::update()
{
    uint32_t currentTime = SDL_GetTicks();

    if (currentTime - startTime > duration)
    {
        active = false;
        entity->removeComponent<FireComponent>();
        return;
    }

    if (currentTime - lastDamageTime >= TIME_SECOND)
    {
        applyFireDamage();
        lastDamageTime = currentTime;
    }

    auto& visual_tr = entity->getComponent<RelationshipComponent>()
                                        .getChild("fire_effect")
                                        ->getComponent<TransformComponent>();

    auto center = target_transform->center();

    auto attachPoint = target_transform->center().subtract({32, 0});
    Vector2D rotatedCenter = Math2D::rotate_point(
        center, 
        target_transform->rotation, 
        attachPoint
    );
    
    visual_tr.centerOn(rotatedCenter);
    visual_tr.rotation = target_transform->rotation - 90;
}

void FireComponent::createFireVisual()
{
    if (!entity->hasComponent<RelationshipComponent>())
    {
        entity->addComponent<RelationshipComponent>();
    }

    auto &fireVisual = entity->m_manager.addEntity();
    fireVisual.addComponent<TransformComponent>(target_transform->pos.x, target_transform->pos.y, 64, 64);
    fireVisual.addComponent<SpriteComponent>("assets/flames.png")
        .setSrcRect({0, 0, 64, 64})
        .addAnimation("burn", {0, 0, 4, 50})
        .play("burn");
    fireVisual.addComponent<DecayComponent>(duration, duration);
    fireVisual.addGroup(groupFlames);

    entity->getComponent<RelationshipComponent>().addChildren(&fireVisual, "fire_effect");
}

void FireComponent::applyFireDamage()
{
    if (entity->hasComponent<DamageModelComponent>())
    {
        auto &damage = entity->getComponent<DamageModelComponent>();
        damage.health -= damagePerSecond;
    }
}