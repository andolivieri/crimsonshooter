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
    void init() override;
    void update() override;
    void draw() override;

private:

    SDL_Rect collider;
    std::string tag;

    TransformComponent* transform;
    SpriteComponent* sprite;
    void handleInput(double angle);
    SDL_Point getPlayerCenter();
};

#endif // INPUTCOMPONENT_H
