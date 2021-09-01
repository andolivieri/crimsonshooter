#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include <string>
#include <SDL.h>
#include "ecs.h"
#include <map>
#include "transformcomponent.h"
#include "../game.h"

typedef enum {
    BTN_UP,
    BTN_RIGHT_UP,
    BTN_RIGHT,
    BTN_RIGHT_DOWN,
    BTN_DOWN,
    BTN_LEFT_DOWN,
    BTN_LEFT,
    BTN_LEFT_UP,
    BTN_FIRE_1,
    BTN_FIRE_2,
    BTN_JUMP,
    BTN_RUN,
    BTN_LAST
} PlayerControls;


class InputComponent : public Component
{

public:
    void init() override;
    void update() override;
    void draw() override;

private:

    SDL_Rect collider;
    std::string tag;
    double strafeAngle;

    TransformComponent* transform;
    SpriteComponent* sprite;
    void handleInput();
    SDL_Point getPlayerCenter();

    std::set<SDL_Keycode> lastKeys;
    std::set<Uint8> lastMouse;


    bool keyReleased(SDL_Keycode );
    bool keyPressedNow(SDL_Keycode k);


    std::map<SDL_Keycode, PlayerControls> keymapping = {
        {SDLK_UP, BTN_UP},
        {SDLK_w, BTN_UP},
        {SDLK_DOWN, BTN_DOWN},
        {SDLK_s, BTN_DOWN},
        {SDLK_LEFT, BTN_LEFT},
        {SDLK_a, BTN_LEFT},
        {SDLK_RIGHT, BTN_RIGHT},
        {SDLK_d, BTN_RIGHT},
        {SDLK_LSHIFT, BTN_RUN},
        {SDLK_RSHIFT, BTN_RUN}
    };

    std::set<PlayerControls> readControls();
};

#endif // INPUTCOMPONENT_H
