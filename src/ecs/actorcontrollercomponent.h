#ifndef ACTORCONTROLLERCOMPONENT_H
#define ACTORCONTROLLERCOMPONENT_H

#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"
#include "inputcomponent.h"


class ActorControllerComponent : public Component
{
public:

    float speed = 2;

    ActorControllerComponent(){}


    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        input = &entity->getComponent<InputComponent>();
    }

    void update() override
    {
        float speedMultiplier = 1;
        for(auto evt : input->frameEvents)
        {
            if(evt.button == BTN_RUN)
            {
                speedMultiplier = 2;
            }

            if(evt.button == BTN_UP)
            {
                transform->velocity.y = (evt.evt == BTN_PRESS ? -speed : 0);
            }

            if(evt.button == BTN_DOWN)
            {
                transform->velocity.y = (evt.evt == BTN_PRESS ? speed : 0);
            }


            if(evt.button == BTN_RIGHT)
            {
                transform->velocity.x = (evt.evt == BTN_PRESS ? speed : 0);
            }


            if(evt.button == BTN_LEFT)
            {
                transform->velocity.x = (evt.evt == BTN_PRESS ? -speed : 0);
            }
        }

         transform->velocity.x *= speedMultiplier;
         transform->velocity.y *= speedMultiplier;
    }


private:
    InputComponent* input;
    TransformComponent* transform;

};

#endif // ACTORCONTROLLERCOMPONENT_H
