#include "aicomponent.h"
#include "engine/game.h"
#include "helpers/utils.h"

void AIComponent::update()
{

    Entity* player = entity->m_manager.getGroup(groupPlayers)[0];

    if(damage->isDead()){

        if(damage->diedNow()){
            if(damage->wasCriticalHit()){
                sprite->play("critical", 1);
            } else {
                sprite->play("dying", 1);
            }
            transform->velocity.x = 0;
            transform->velocity.y = 0;
            entity->delGroup(groupEnemies);
            entity->addGroup(groupDeadEnemies);
            family->getChild("shadow")->setActive(false);
            entity->addComponent<DecayComponent>(ONE_MINUTE, ONE_MINUTE - 5*ONE_SECOND);
            entity->removeComponent<ColliderComponent>();

        }
        return;
    }


    if(player->getComponent<DamageModelComponent>().isDead()){
        sprite->play("idle");
        transform->velocity = {0,0};
        return;
    }

    if (isOnFire()) {
        updateFireBehavior();
        return;
    }

    Vector2D target = player->getComponent<TransformComponent>().pos;

    if(transform->pos.x > target.x)
        transform->velocity.x = -speed;
    else if (transform->pos.x < target.x)
        transform->velocity.x = speed;
    else
        transform->velocity.x = 0;

    if(transform->pos.y > target.y)
        transform->velocity.y = -speed;
    else if (transform->pos.y < target.y)
        transform->velocity.y = speed;
    else
        transform->velocity.y = 0;

    if(transform->velocity.x || transform->velocity.y )
    {
        sprite->play("moving");
    }else{
        sprite->play("idle");
    }

    transform->rotation = Math2D::angleBetweenPoints(transform->pos, target);

}

void AIComponent::randomtarget()
{
    static uint32_t lastChange = 0;
    if(SDL_GetTicks() - lastChange > 30)
    {
        randtarget = {rand() % Game::winWidth, rand() & Game::winHeigth};
        lastChange = SDL_GetTicks();
    }
}

bool AIComponent::isOnFire()
{
    return entity->hasComponent<FireComponent>() && 
           entity->getComponent<FireComponent>().isOnFire();
}

void AIComponent::updateFireBehavior()
{
    uint32_t currentTime = SDL_GetTicks();
    
    // Change direction every 2 seconds when on fire
    if (currentTime - lastFireDirectionChange > 2000) {
        float angle = (rand() % 360) * M_PI / 180.0f;
        fireDirection.x = cos(angle);
        fireDirection.y = sin(angle);
        lastFireDirectionChange = currentTime;
    }
    
    // Move faster when on fire (2x speed)
    float fireSpeed = 2.5;
    transform->velocity.x = fireDirection.x * fireSpeed;
    transform->velocity.y = fireDirection.y * fireSpeed;
    
    // Rotate the enemy to face the movement direction
    transform->rotation = Math2D::angleBetweenPoints({0, 0}, fireDirection);
    
    sprite->play("moving");
}
