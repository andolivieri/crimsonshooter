#include "inputcomponent.h"
#include <cmath>
#include "math2d.h"
#include "texturemanager.h"
#include "utils.h"
#include "../collision.h"
#include "../game.h"

void InputComponent::init()
{
    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    SDL_SetCursor(cursor);
    SDL_ShowCursor(1);

    transform = &entity->emplaceComponent<TransformComponent>();
    sprite = &entity->getComponent<SpriteComponent>();

}

void InputComponent::update()
{

    frameEvents.clear();

    DamageModelComponent& damage = entity->getComponent<DamageModelComponent>();

    if(damage.isDead())
    {
        sprite->play("dead",1);
        transform->velocity.x = 0;
        transform->velocity.y = 0;
        return;
    }

    SDL_Point mousePt;
    SDL_GetMouseState(&mousePt.x,&mousePt.y);

    SDL_Point playerPt = getPlayerCenter();

    double angle = Math2D::angleBetweenPoints(playerPt, mousePt);
    transform->rotation = angle;

    handleInput();


    // TODO handle collision here but we should really not
    for(auto &enemy : entity->m_manager.getGroup(groupEnemies))
    {
        if(Collision::AABB(entity->getComponent<ColliderComponent>(),
                           enemy->getComponent<ColliderComponent>()))
        {
            damage.health = damage.health - 10;
            std::cout << "Player hit. Healt=" << damage.health <<  std::endl;
        }
    }


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

bool InputComponent::keyPressed(SDL_Keycode k)
{
    return Game::pressedKeys.count(k);
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
void InputComponent::handleInput()
{

    const std::set<uint8_t>& mouse = Game::pressedMouseButtons;

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


    // n.b. ordine matters
    float speedMult = 1;
    //if(pressedkeys.count(BTN_RUN))
      //  speedMult = 2;

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
    transform->pos.x = std::min<float>(transform->pos.x, static_cast<float>(Game::winWidth) - transform->width);

    transform->pos.y = std::max<float>(transform->pos.y, 0);
    transform->pos.y = std::min<float>(transform->pos.y, static_cast<float>(Game::winHeigth) - transform->height);


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
