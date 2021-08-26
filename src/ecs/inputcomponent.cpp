#include "inputcomponent.h"
#include <cmath>
#include "math2d.h"
#include "texturemanager.h"
#include "utils.h"
double aangle;

void InputComponent::init()
{
    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    SDL_SetCursor(cursor);
    SDL_ShowCursor(1);

    if(!entity->hasComponent<TransformComponent>())
    {
        entity->addComponent<TransformComponent>();
    }
    transform = &entity->getComponent<TransformComponent>();
}

void InputComponent::update()
{
    SDL_Point mousePt;
    SDL_GetMouseState(&mousePt.x,&mousePt.y);

    SDL_Point playerPt = getPlayerCenter();

    double angle = Math2D::angleBetweenPoints(playerPt, mousePt);

    transform->rotation = angle;

    handleInput(angle);
}

void InputComponent::draw()
{
#ifdef _DEBUG
    SDL_Point p = getPlayerCenter();
    SDL_Color col;
    memset(&col, 0, sizeof(SDL_Color));
    col.r = 255;
    drawCircle(TextureManager::renderer, p, 3, col);
#endif
}


SDL_Point InputComponent::getPlayerCenter()
{
    SDL_Point playerPt;
    playerPt.x = transform->pos.x + (transform->scaledWidth() / 2);
    playerPt.y = transform->pos.y + (transform->scaledHeight()) / 2;
    return playerPt;
}

void InputComponent::handleInput(double angle)
{
    const std::set<SDL_Keycode>& gg = Game::pressedKeys;

    transform->velocity.x = 0;
    transform->velocity.y = 0;

    if(gg.count(SDLK_LEFT) || gg.count(SDLK_a))
    {
        transform->velocity.x = -(float)transform->speed;
    }

    if(gg.count(SDLK_RIGHT)|| gg.count(SDLK_d))
    {
        transform->velocity.x = (float)transform->speed;
    }

    if(gg.count(SDLK_UP)|| gg.count(SDLK_w))
    {
        int xdirection = (angle <= 90 && angle >= -90) ? -1 : 1;
        int ydirection = (angle <= 180 && angle >= 0) ? 1 : -1;
        xdirection = ydirection = 1;

        double radAngle = Math2D::deg2rad(angle);

        transform->velocity.y = transform->speed * std::sin(radAngle) * ydirection;
        transform->velocity.x = transform->speed * std::cos(radAngle) * xdirection;
    }

    if(gg.count(SDLK_DOWN)|| gg.count(SDLK_s))
    {
        transform->velocity.y = (float)transform->speed;
    }

}
