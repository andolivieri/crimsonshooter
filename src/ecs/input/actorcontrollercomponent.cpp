#include "actorcontrollercomponent.h"
#include "engine/game.h"
#include "engine/gamemap.h"
#include "helpers/collision.h"
#include "helpers/utils.h"


void ActorControllerComponent::update()
{



    if(damage->isDead())
    {
        if(damage->diedNow()){
            std::cout << "died now" << std::endl;
            entity->addComponent<TimerComponent>(5 * ONE_SECOND).onTrigger([&](Entity& _){
                this->fart(true);
            });
        }
        sprite->play("dead",1);
        entity->removeComponent<ColliderComponent>();

        transform->velocity.x = 0;
        transform->velocity.y = 0;
        wbay->drop(0);
        wbay->drop(1);
        return;
    }


    if(input->hasJoyAim)
    {
        transform->rotation = input->joyAimAngle;
    }
    else
    {
        SDL_Point mousePt;
        SDL_GetMouseState(&mousePt.x,&mousePt.y);

        SDL_Point playerPt;
        playerPt.x = static_cast<int>(transform->center().x  - Game::camera.x);
        playerPt.y = static_cast<int>(transform->center().y - Game::camera.y);

        double angle = Math2D::angleBetweenPoints(playerPt, mousePt);
        transform->rotation = angle;
    }


    float speedMultiplier = 1;
    bool tryingToSprint = false;
    
    for(auto evt : input->frameEvents)
    {

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

        if(evt.button == BTN_RUN && evt.evt == BTN_PRESS)
        {
            tryingToSprint = true;
        }

        if(evt.button == BTN_FIRE_1)
            evt.evt == BTN_PRESS ? wbay->triggerPull(0) : wbay->triggerRelease(0);
        if(evt.button == BTN_FIRE_2)
            evt.evt == BTN_PRESS ? wbay->triggerPull(1) : wbay->triggerRelease(1);
        if(evt.button == BTN_RELOAD && evt.evt == BTN_PRESS)
        {
            wbay->reload(0);
            wbay->reload(1);
        }
    }
    
    // stamina
    uint32_t currentTime = SDL_GetTicks();
    static uint32_t lastStaminaUpdate = currentTime;
    float deltaTime = (currentTime - lastStaminaUpdate) / 1000.0f;
    lastStaminaUpdate = currentTime;
    
    bool isMoving = (transform->velocity.x != 0 || transform->velocity.y != 0);
    bool canSprint = stamina->canSprint();
    
    if(tryingToSprint && isMoving && canSprint) {
        speedMultiplier = 2;
        stamina->consumeStamina(deltaTime);
    } else {
        stamina->rechargeStamina(deltaTime);
    }

    transform->velocity.x *= speedMultiplier;
    transform->velocity.y *= speedMultiplier;


    // Let's not allow the player off screen

   transform->pos.x = std::max<float>(transform->pos.x, 0);
   transform->pos.x = std::min<float>(transform->pos.x, static_cast<float>(GameMap::mapWidth) - transform->width);

   transform->pos.y = std::max<float>(transform->pos.y, 0);
   transform->pos.y = std::min<float>(transform->pos.y, static_cast<float>(GameMap::mapHeight) - transform->height);



    if(transform->velocity.y || transform->velocity.x){
        if(speedMultiplier > 1)
            sprite->play("fast");
        else
            sprite->play("moving");
    }else{
        sprite->play("idle");
    }

}

void ActorControllerComponent::fart(bool force)
{
    static const auto FART_COOLDOWN = 15 * ONE_SECOND;
    static const uint16_t FART_DECAY = 8 * ONE_SECOND;
    static auto lastFart = -FART_COOLDOWN;
    if (SDL_GetTicks() - lastFart > FART_COOLDOWN ||  force) {
        lastFart = SDL_GetTicks();
        std::string whichFart = "assets/sounds/fart" + std::to_string(rand() % 5) + ".ogg";
        this->sound->play(whichFart, 0, 3);

        auto& fart = entity->m_manager.addEntity();

        auto fartTr = *transform;
        fartTr.centerOn(transform->center());
        fartTr.velocity.x /= 10;
        fartTr.velocity.y /= 10;
        fart.addComponent<TransformComponent>(fartTr);

        fart.addComponent<SpriteComponent>("assets/smoke.png")
                .setSrcRect({0,0,64,64})
                .addAnimation("fog", {0, 0, 8, 800 })
                .play("fog",1);

        fart.addComponent<ColliderComponent>().onCollision([&](Entity& t)
        {
            if(t.hasComponent<DamageModelComponent>()){
                auto damage = 1;
                DamageModelComponent& enemyDamage = t.getComponent<DamageModelComponent>();
                enemyDamage.health -= damage;
            }
        });

        fart.addComponent<TimerComponent>(ONE_SECOND / 3, true).onTrigger([&](Entity& e){
            const auto coeff =  1.07;
            e.getComponent<TransformComponent>().width *= coeff;
            e.getComponent<TransformComponent>().height *= coeff;
        });

        fart.addComponent<DecayComponent>(FART_DECAY, FART_DECAY - 2*ONE_SECOND);
        fart.addGroup(groupProjectiles);

    }
}
