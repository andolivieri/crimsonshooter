#include "inputcomponent.h"
#include <cmath>
#include "helpers/math2d.h"
#include "engine/texturemanager.h"
#include "helpers/utils.h"
#include "helpers/collision.h"
#include "engine/game.h"

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
    handleJoystick();

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

void InputComponent::handleJoystick()
{
    hasJoyAim = false;

    SDL_GameController* pad = Game::gameController;
    if(!pad)
    {
        lastJoyButtons.clear();
        return;
    }

    // SDL analog axes range over [-32768, 32767].
    static const int MOVE_DEADZONE = 8000;
    static const int AIM_DEADZONE  = 8000;

    std::set<int> active;

    // ---- Left stick: movement, treated as a d-pad (no sensitivity) ----
    int lx = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX);
    int ly = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY);
    if(lx < -MOVE_DEADZONE) active.insert(BTN_LEFT);
    if(lx >  MOVE_DEADZONE) active.insert(BTN_RIGHT);
    if(ly < -MOVE_DEADZONE) active.insert(BTN_UP);
    if(ly >  MOVE_DEADZONE) active.insert(BTN_DOWN);

    // ---- Face/shoulder buttons: L1/R1 fire primary/secondary, X grenade ----
    // "X" is the south face button (Cross on PlayStation, A on Xbox).
    if(SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
        active.insert(BTN_FIRE_1);
    if(SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
        active.insert(BTN_FIRE_2);
    // Triggers are analog ([0, 32767]); treat past-halfway as pressed.
    // R2 -> grenade (hold to charge, release to throw), L2 -> sprint.
    if(SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 16000)
        active.insert(BTN_GRENADE);
    if(SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 16000)
        active.insert(BTN_RUN);

    // Emit PRESS every frame while active (mirrors how the keyboard/mouse are
    // polled above) and a single RELEASE on the active->inactive transition.
    for(int btn : active)
    {
        InputEvent e;
        e.button = static_cast<PlayerControl>(btn);
        e.evt = BTN_PRESS;
        frameEvents.push_back(e);
    }
    for(int btn : lastJoyButtons)
    {
        if(active.count(btn) == 0)
        {
            InputEvent e;
            e.button = static_cast<PlayerControl>(btn);
            e.evt = BTN_RELEASE;
            frameEvents.push_back(e);
        }
    }
    lastJoyButtons = active;

    // ---- Right stick: aim ----
    int rx = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTX);
    int ry = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTY);
    if(std::abs(rx) > AIM_DEADZONE || std::abs(ry) > AIM_DEADZONE)
    {
        hasJoyAim = true;
        joyAimAngle = atan2(static_cast<double>(ry),
                            static_cast<double>(rx)) * 180.0 / M_PI;
    }
}
