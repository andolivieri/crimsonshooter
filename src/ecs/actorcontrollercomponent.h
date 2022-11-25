#ifndef ACTORCONTROLLERCOMPONENT_H
#define ACTORCONTROLLERCOMPONENT_H

#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"
#include "inputcomponent.h"
#include "spritecomponent.h"
#include "damagemodel.h"
#include "weaponbaycomponent.h"


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
        wbay = &entity->getComponent<WeaponBayComponent>();
        sound =  &entity->getComponent<SoundComponent>();
    }

    void update() override;


private:
    void fart();

    DamageModelComponent* damage;
    InputComponent* input;
    TransformComponent* transform;
    SpriteComponent* sprite;
    WeaponBayComponent* wbay;
    SoundComponent* sound;

};

#endif // ACTORCONTROLLERCOMPONENT_H
