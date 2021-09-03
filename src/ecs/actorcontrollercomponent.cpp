#include "actorcontrollercomponent.h"
#include "../game.h"
#include "../collision.h"


void ActorControllerComponent::update()
{

    SDL_Point mousePt;
    SDL_GetMouseState(&mousePt.x,&mousePt.y);

    SDL_Point playerPt;
    playerPt.x = transform->center().x;
    playerPt.y = transform->center().y;

    double angle = Math2D::angleBetweenPoints(playerPt, mousePt);
    transform->rotation = angle;


    // TODO handle collision here but we should really not
    for(auto &enemy : entity->m_manager.getGroup(groupEnemies))
    {
        if(Collision::AABB(entity->getComponent<ColliderComponent>(),
                           enemy->getComponent<ColliderComponent>()))
        {
            damage->health -= 10;
            std::cout << "Player hit. Health=" << damage->health <<  std::endl;
        }
    }

    if(damage->isDead())
    {
        sprite->play("dead",1);
        transform->velocity.x = 0;
        transform->velocity.y = 0;
        return;
    }

    float speedMultiplier = 1;
    for(auto evt : input->frameEvents)
    {
        if(evt.button == BTN_RUN)
        {
            speedMultiplier = 2;
        }

        if(evt.button == BTN_UP)
        {
            transform->velocity.y = (evt.evt == BTN_PRESS ? -speed : 0);
        }

        if(evt.button == BTN_DOWN)
        {
            transform->velocity.y = (evt.evt == BTN_PRESS ? speed : 0);
        }

        if(evt.button == BTN_RIGHT)
        {
            transform->velocity.x = (evt.evt == BTN_PRESS ? speed : 0);
        }


        if(evt.button == BTN_LEFT)
        {
            transform->velocity.x = (evt.evt == BTN_PRESS ? -speed : 0);
        }

    }

    transform->velocity.x *= speedMultiplier;
    transform->velocity.y *= speedMultiplier;


    // Let's not allow the player off screen

    transform->pos.x = std::max<float>(transform->pos.x, 0);
    transform->pos.x = std::min<float>(transform->pos.x, static_cast<float>(Game::winWidth) - transform->width);

    transform->pos.y = std::max<float>(transform->pos.y, 0);
    transform->pos.y = std::min<float>(transform->pos.y, static_cast<float>(Game::winHeigth) - transform->height);



    if(transform->velocity.y || transform->velocity.x){
        if(speedMultiplier > 1)
            sprite->play("fast");
        else
            sprite->play("moving");
    }else{
        sprite->play("idle");
    }

}
