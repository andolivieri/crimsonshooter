#ifndef ACTORCONTROLLERCOMPONENT_H
#define ACTORCONTROLLERCOMPONENT_H

#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"
#include "inputcomponent.h"
#include "spritecomponent.h"
#include "damagemodel.h"
#include "weaponbaycomponent.h"
#include "staminacomponent.h"
#include "utils.h"


class ActorControllerComponent : public Component
{
public:

    float speed = 2;
    int maxSprintMsec = 3 * TIME_SECOND;
    int currentSprintMsec = 3*TIME_SECOND;
    uint32_t lastSprintStart = 0;

    ActorControllerComponent(){}


    void init() override
    {
        sprite = &entity->getComponent<SpriteComponent>();
        damage = &entity->getComponent<DamageModelComponent>();
        transform = &entity->getComponent<TransformComponent>();
        input = &entity->getComponent<InputComponent>();
        wbay = &entity->getComponent<WeaponBayComponent>();
        sound =  &entity->getComponent<SoundComponent>();
        stamina = &entity->getComponent<StaminaComponent>();
    }

    void update() override;


private:
    void fart(bool force=false);

    DamageModelComponent* damage;
    InputComponent* input;
    TransformComponent* transform;
    SpriteComponent* sprite;
    WeaponBayComponent* wbay;
    SoundComponent* sound;
    StaminaComponent* stamina;

};

#endif // ACTORCONTROLLERCOMPONENT_H
