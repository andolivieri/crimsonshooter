#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include <string>
#include <SDL.h>
#include "ecs.h"
#include <map>
#include <deque>
#include "transformcomponent.h"
#include "spritecomponent.h"
#include "damagemodel.h"

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
    BTN_RELOAD,
    BTN_LAST
} PlayerControls;

typedef enum {
    BTN_PRESS,
    BTN_RELEASE
} InputEventType;

struct InputEvent
{
    PlayerControls button;
    InputEventType evt;
};


class InputComponent : public Component
{

public:
    void init() override;
    void update() override;
    void draw() override;

    std::deque<InputEvent> frameEvents;

private:


    void handleInput();

    std::set<SDL_Keycode> lastKeys;
    std::set<Uint8> lastMouse;

    bool keyReleased(SDL_Keycode );
    bool keyPressedNow(SDL_Keycode k);
    bool keyPressed(SDL_Keycode k);
    bool mouseReleased(Uint8 k);
    bool mousePressed(Uint8 k);


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
        {SDLK_RSHIFT, BTN_RUN},
        {SDLK_r, BTN_RELOAD},
    };


};

#endif // INPUTCOMPONENT_H
