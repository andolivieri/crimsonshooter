#include "firecomponent.h"
#include "ecs/components.h"
#include "engine/game.h"
#include <iostream>

FireComponent::FireComponent(float duration, int damagePerSecond)
    : duration(duration), damagePerSecond(damagePerSecond)
{
}

void FireComponent::init()
{

    transform = &entity->getComponent<TransformComponent>();
    damageModel = &entity->getComponent<DamageModelComponent>();
    sprite = &entity->getComponent<SpriteComponent>();
    relationship = &entity->getComponent<RelationshipComponent>();
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

    if (currentTime - lastDamageTime >= ONE_SECOND)
    {
        damageModel->health -= damagePerSecond;
        lastDamageTime = currentTime;
    }
    
    // TODO andoli: relationships are BROKEN.
    // here fire_effect has already been removed by DecayComponent

    if(!relationship->hasChildren("fire_effect"))
        return;
    
    auto fireVisual = relationship->getChild("fire_effect");

    auto &visualTr = fireVisual->getComponent<TransformComponent>();

    auto center = transform->center();

    if(damageModel->diedNow()){
        //fireVisual->emplaceComponent<DecayComponent>(3*ONE_SECOND);
        visualTr.centerOn(center.subtract({0, 32}));
        visualTr.rotation = 0;
        sprite->setColorMod(128,128,128);
    }else if(!damageModel->isDead()){
        const auto attachPoint = transform->center().subtract({32, 0});
        Vector2D rotatedCenter = Math2D::rotate_point(
            center,
            transform->rotation,
            attachPoint);
        visualTr.centerOn(rotatedCenter);
        visualTr.rotation = transform->rotation - 90; // sucks but foe sprite is rotated 90 clockwise
    }

}

void FireComponent::createFireVisual()
{
    
    auto &fireVisual = entity->m_manager.addEntity();
    fireVisual.addComponent<TransformComponent>(transform->pos.x, transform->pos.y, 64, 64);
    fireVisual.addComponent<SpriteComponent>("assets/flames.png")
        .setSrcRect({0, 0, 64, 64})
        .addAnimation("burn", {0, 0, 4, 50})
        .play("burn");
    fireVisual.addComponent<DecayComponent>(duration, duration * 0.8);
    fireVisual.addGroup(groupFlames);

    relationship->addChildren(&fireVisual, "fire_effect");
}