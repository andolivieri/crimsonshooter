#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include <string>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"
#include "../game.h"


class InputComponent : public Component
{

public:
    void init() override
    {
        if(!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {

        switch (Game::event.type) {
        case SDL_KEYDOWN:
            onKeyDown(&Game::event.key);
            break;
        case SDL_KEYUP:
            onKeyUp(&Game::event.key);
            break;
        default:
            break;
        }
    }

private:

    SDL_Rect collider;
    std::string tag;

    TransformComponent* transform;
    void onKeyDown(SDL_KeyboardEvent *key);
    void onKeyUp(SDL_KeyboardEvent *key);
};

#endif // INPUTCOMPONENT_H
