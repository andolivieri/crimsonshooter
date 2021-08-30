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
    sprite = &entity->getComponent<SpriteComponent>();
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
#ifdef __DEBUG
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
    playerPt.x = static_cast<int>(transform->pos.x + (transform->scaledWidth() / 2));
    playerPt.y = static_cast<int>(transform->pos.y + (transform->scaledHeight()) / 2);
    return playerPt;
}

void InputComponent::handleInput(double angle)
{

    const std::set<SDL_Keycode>& gg = Game::pressedKeys;
    const std::set<uint8_t>& mouse = Game::pressedMouseButtons;

    transform->velocity.x = 0;
    transform->velocity.y = 0;


    double radAngle = Math2D::deg2rad(angle);

    // n.b. ordine
    float speedMult = 1;
    if(gg.count(SDLK_LSHIFT)|| gg.count(SDLK_RSHIFT))
        speedMult = 2;

    if(gg.count(SDLK_UP)|| gg.count(SDLK_w))
    {
        transform->velocity.y = transform->speed * static_cast<float>(std::sin(radAngle)) * speedMult;
        transform->velocity.x = transform->speed * static_cast<float>(std::cos(radAngle))* speedMult;
    }

    if(gg.count(SDLK_DOWN)|| gg.count(SDLK_s))
    {
        transform->velocity.y = -transform->speed * static_cast<float>(std::sin(radAngle))* speedMult;
        transform->velocity.x = -transform->speed * static_cast<float>(std::cos(radAngle))* speedMult;
    }


    if(gg.count(SDLK_LEFT) || gg.count(SDLK_a))
    {
        transform->velocity.y = -transform->speed * static_cast<float>(std::sin(radAngle + M_PI/2))* speedMult;
        transform->velocity.x = -transform->speed * static_cast<float>(std::cos(radAngle + M_PI/2))* speedMult;
    }

    if(gg.count(SDLK_RIGHT)|| gg.count(SDLK_d))
    {
        transform->velocity.y = transform->speed * static_cast<float>(std::sin(radAngle + M_PI/2))* speedMult;
        transform->velocity.x = transform->speed * static_cast<float>(std::cos(radAngle + M_PI/2))* speedMult;
    }

    if(transform->velocity.y || transform->velocity.x){
        if(speedMult > 1)
            sprite->play("fast");
        else
            sprite->play("moving");
    }else{
        sprite->play("idle");
    }


    if(entity->hasComponent<WeaponComponent>())
    {
        WeaponComponent& w = entity->getComponent<WeaponComponent>();
        if(mouse.count(SDL_BUTTON_LEFT))
            w.triggerPull();
        else
            w.triggerRelease();

    }



}
