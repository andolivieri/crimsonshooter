#include "firecomponent.h"
#include "ecs/components.h"
#include "game.h"
#include <iostream>

FireComponent::FireComponent(float duration, int damagePerSecond) 
    : duration(duration), damagePerSecond(damagePerSecond) {
}

void FireComponent::init() {
    if (!entity->hasComponent<TransformComponent>()) {
        return;
    }
    
    target_transform = &entity->getComponent<TransformComponent>();
    startTime = SDL_GetTicks();
    lastDamageTime = startTime;
    
    createFireVisual();
}

void FireComponent::update() {
    uint32_t currentTime = SDL_GetTicks();
    
    if (currentTime - startTime > duration) {
        active = false;
        entity->removeComponent<FireComponent>();
        return;
    }

    if (currentTime - lastDamageTime >= TIME_SECOND) {
        applyFireDamage();
        lastDamageTime = currentTime;
    }

    TransformComponent& visual_tr = entity->getComponent<RelationshipComponent>()
        .getChild("fire_effect")
        ->getComponent<TransformComponent>();

    visual_tr.centerOn(target_transform->center().subtract({0, 50}));
    
}

void FireComponent::createFireVisual() {
    if (!entity->hasComponent<RelationshipComponent>()) {
        entity->addComponent<RelationshipComponent>();
    }
    
    auto& fireVisual = entity->m_manager.addEntity();
    fireVisual.addComponent<TransformComponent>(target_transform->pos.x, target_transform->pos.y, 64, 134);
    fireVisual.addComponent<SpriteComponent>("assets/fire_anim.png")
            .setSrcRect({0, 0, 64, 134})
            .setAlpha(180)
            .addAnimation("burn", {0, 0, 8, 50})
            .play("burn");
    fireVisual.addComponent<DecayComponent>(duration);
    fireVisual.addGroup(groupAuras);
    
    entity->getComponent<RelationshipComponent>().addChildren(&fireVisual, "fire_effect");
}

void FireComponent::applyFireDamage() {
    if (entity->hasComponent<DamageModelComponent>()) {
        auto& damage = entity->getComponent<DamageModelComponent>();
        damage.health -= damagePerSecond;
    }
}