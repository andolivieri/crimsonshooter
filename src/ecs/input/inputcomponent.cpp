#include "inputcomponent.h"
#include <cmath>
#include "math2d.h"
#include "texturemanager.h"
#include "utils.h"
#include "collision.h"
#include "game.h"

void InputComponent::init()
{
    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    SDL_SetCursor(cursor);
    SDL_ShowCursor(1);
}

void InputComponent::update()
{

    frameEvents.clear();

    handleInput();

}

void InputComponent::draw()
{
#ifdef __DEBUG
    SDL_Point p = getPlayerCenter();
    SDL_Color col;
    memset(&col, 0, sizeof(SDL_Color));
    col.r = 255;
    drawCircle(TextureManager::renderer, p, 3, col);
#endif
}


bool InputComponent::keyReleased(SDL_Keycode k)
{
    return lastKeys.count(k) && Game::pressedKeys.count(k) == 0;
}

bool InputComponent::keyPressed(SDL_Keycode k)
{
    return Game::pressedKeys.count(k);
}

bool InputComponent::keyPressedNow(SDL_Keycode k)
{
    return lastKeys.count(k) == 0 && Game::pressedKeys.count(k);
}

bool InputComponent::mousePressed(Uint8 k)
{
    return Game::pressedMouseButtons.count(k);
}

bool InputComponent::mouseReleased(Uint8 k)
{
    return lastMouse.count(k)&& Game::pressedMouseButtons.count(k) == 0;
}

void InputComponent::handleInput()
{

    for(auto kv : keymapping)
    {
        if(keyPressed(kv.first))
        {
            InputEvent e;
            e.button = kv.second;
            e.evt = BTN_PRESS;
            frameEvents.push_back(e);
        }

        if(keyReleased(kv.first))
        {
            InputEvent e;
            e.button = kv.second;
            e.evt = BTN_RELEASE;
            frameEvents.push_back(e);
        }
    }

    if(mousePressed(SDL_BUTTON_LEFT))
    {
        InputEvent e;
        e.button = BTN_FIRE_1;
        e.evt = BTN_PRESS;
        frameEvents.push_back(e);
    }

    if(mouseReleased(SDL_BUTTON_LEFT))
    {
        InputEvent e;
        e.button = BTN_FIRE_1;
        e.evt = BTN_RELEASE;
        frameEvents.push_back(e);
    }

    if(mousePressed(SDL_BUTTON_RIGHT))
    {
        InputEvent e;
        e.button = BTN_FIRE_2;
        e.evt = BTN_PRESS;
        frameEvents.push_back(e);
    }

    if(mouseReleased(SDL_BUTTON_RIGHT))
    {
        InputEvent e;
        e.button = BTN_FIRE_2;
        e.evt = BTN_RELEASE;
        frameEvents.push_back(e);
    }

    lastKeys = Game::pressedKeys;
    lastMouse = Game::pressedMouseButtons;

}
