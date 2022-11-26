#include "actorcontrollercomponent.h"
#include "../game.h"
#include "../collision.h"


void ActorControllerComponent::update()
{


    if(damage->isDead())
    {
        sprite->play("dead",1);

        transform->velocity.x = 0;
        transform->velocity.y = 0;
        wbay->drop(0);
        wbay->drop(1);
        return;
    }


    SDL_Point mousePt;
    SDL_GetMouseState(&mousePt.x,&mousePt.y);

    SDL_Point playerPt;
    playerPt.x = static_cast<int>(transform->center().x);
    playerPt.y = static_cast<int>(transform->center().y);

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

    // TODO handle collision here but we should really not
    for(auto &perk : entity->m_manager.getGroup(groupPerks))
    {
        if(Collision::AABB(entity->getComponent<ColliderComponent>(),
                           perk->getComponent<ColliderComponent>()))
        {
            wbay->autoequip(perk->getComponent<PerkComponent>().weapon);
            perk->setActive(false);
        }
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

        if(evt.button == BTN_FART)
        {
            fart();
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

void ActorControllerComponent::fart()
{
    static const uint16_t FART_COOLDOWN_MSEC = 10000;
    static auto lastFart = -FART_COOLDOWN_MSEC;
    if (SDL_GetTicks() - lastFart > FART_COOLDOWN_MSEC) {
        lastFart = SDL_GetTicks();
        std::string whichFart = "assets/sounds/fart" + std::to_string(rand() % 5) + ".wav";
        this->sound->play(whichFart, 0, 3);

        auto& fart = entity->m_manager.addEntity();

        auto fartTr = *transform;
        fartTr.width *= 2;
        fartTr.height *= 2;
        fartTr.centerOn(transform->center());
        fartTr.velocity.x /= 10;
        fartTr.velocity.y /= 10;
        fart.addComponent<TransformComponent>(fartTr);
        fart.addComponent<SpriteComponent>("assets/smoke.png")
                .setSrcRect({0,0, 126,112})
                .addAnimation("fog", {0, rand() % 3, 8, 500 })
                .play("fog",1);
                ;
        fart.addComponent<DecayComponent>(10*1000);
        fart.addComponent<ColliderComponent>().onCollision([&](Entity& t)
        {
            if(t.hasComponent<DamageModelComponent>()){
                DamageModelComponent& enemyDamage = t.getComponent<DamageModelComponent>();
                 enemyDamage.health -= 1;
            }
        });
        fart.addGroup(groupBloodPatches);

    }
}
