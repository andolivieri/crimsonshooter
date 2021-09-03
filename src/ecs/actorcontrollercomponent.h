#ifndef ACTORCONTROLLERCOMPONENT_H
#define ACTORCONTROLLERCOMPONENT_H

#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"
#include "inputcomponent.h"
#include "spritecomponent.h"
#include "damagemodel.h"


class ActorControllerComponent : public Component
{
public:

    float speed = 2;

    ActorControllerComponent(){}


    void init() override
    {

        sprite = &entity->getComponent<SpriteComponent>();
        damage = &entity->getComponent<DamageModelComponent>();
        transform = &entity->getComponent<TransformComponent>();
        input = &entity->getComponent<InputComponent>();
    }

    void update() override;


private:
    DamageModelComponent* damage;
    InputComponent* input;
    TransformComponent* transform;
    SpriteComponent* sprite;

};

#endif // ACTORCONTROLLERCOMPONENT_H
