#include "inputcomponent.h"
#include <cmath>
#include "math2d.h"
#include "texturemanager.h"
#include "utils.h"

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

bool InputComponent::keyReleased(SDL_Keycode k)
{
    return lastKeys.count(k) && Game::pressedKeys.count(k) == 0;
}

bool InputComponent::keyPressedNow(SDL_Keycode k)
{
    return lastKeys.count(k) == 0 && Game::pressedKeys.count(k);
}


std::set<PlayerControls> InputComponent::readControls()
{
    std::set<PlayerControls> controls;
    for(auto k : Game::pressedKeys)
        if(keymapping.count(k))
            controls.insert(keymapping[k]);

    return controls;
}
void InputComponent::handleInput(double angle)
{

    const std::set<SDL_Keycode>& gg = Game::pressedKeys;
    const std::set<uint8_t>& mouse = Game::pressedMouseButtons;

    auto pressedkeys = readControls();

    transform->velocity.x = 0;
    transform->velocity.y = 0;

    // n.b. ordine matters
    float speedMult = 1;
    if(pressedkeys.count(BTN_RUN))
        speedMult = 2;

    if(pressedkeys.count(BTN_UP))
    {
        transform->velocity.y = -transform->speed * speedMult;
    }

    if(pressedkeys.count(BTN_DOWN))
    {
        transform->velocity.y = transform->speed * speedMult;
    }

    if(pressedkeys.count(BTN_LEFT))
    {
        transform->velocity.x = -transform->speed * speedMult;
    }

    if(pressedkeys.count(BTN_RIGHT))
    {
        transform->velocity.x = transform->speed * speedMult;
    }

    if(transform->velocity.y || transform->velocity.x){
        if(speedMult > 1)
            sprite->play("fast");
        else
            sprite->play("moving");
    }else{
        sprite->play("idle");
    }

    // Let's not allow the player off screen

    transform->pos.x = std::max<float>(transform->pos.x, 0);
    transform->pos.x = std::min<float>(transform->pos.x, Game::winWidth - transform->width);

    transform->pos.y = std::max<float>(transform->pos.y, 0);
    transform->pos.y = std::min<float>(transform->pos.y, Game::winHeigth - transform->height);


    if(entity->hasComponent<WeaponComponent>())
    {
        WeaponComponent& w = entity->getComponent<WeaponComponent>();
        if(mouse.count(SDL_BUTTON_LEFT))
            w.triggerPull();
        else
            w.triggerRelease();

    }

    lastKeys = Game::pressedKeys;
    lastMouse = Game::pressedMouseButtons;

}
