#include "inputcomponent.h"


void InputComponent::onKeyDown(SDL_KeyboardEvent *key)
{
    SDL_Keycode kc = key->keysym.sym;

    std::cout << "Key pressed: " << SDL_GetKeyName(kc)  << std::endl;
    int speed = 3;

    switch (kc) {
    case SDLK_LEFT:
    case SDLK_a:
        transform->velocity.x = -(float)speed;
        break;
    case SDLK_RIGHT:
    case SDLK_d:
        transform->velocity.x = (float)speed;
        break;

    case SDLK_UP:
    case SDLK_w:
        transform->velocity.y = -(float)speed;
        break;

    case SDLK_DOWN:
    case SDLK_s:
        transform->velocity.y = (float)speed;
        break;
    default:
        break;
    }
}


void InputComponent::onKeyUp(SDL_KeyboardEvent *key)
{
    SDL_Keycode kc = key->keysym.sym;
    switch (kc) {
    case SDLK_LEFT:
    case SDLK_a:
        transform->velocity.x = 0;
        break;
    case SDLK_RIGHT:
    case SDLK_d:
        transform->velocity.x = 0;
        break;

    case SDLK_UP:
    case SDLK_w:
        transform->velocity.y = 0;
        break;

    case SDLK_DOWN:
    case SDLK_s:
        transform->velocity.y = 0;
        break;
    default:
        break;
    }
}
