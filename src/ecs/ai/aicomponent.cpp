#include "aicomponent.h"
#include "engine/game.h"
#include "helpers/utils.h"
#include <cmath>

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
            entity->addComponent<BackgroundStickerComponent>(groupDeadEnemies);
            entity->removeComponent<ColliderComponent>();

            sprite->setOnAnimationEnd([this]{ 
                entity->destroy();
             });
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
    onFireLastFrame = false;

    Vector2D target = player->getComponent<TransformComponent>().pos;

    // Ask the pathfinder for a route around obstacles; steer toward the next
    // waypoint. If no path is available (off-map, unreachable, or already in the
    // player's tile) fall back to straight-line chase so the enemy never freezes.
    Vector2D playerCenter = player->getComponent<TransformComponent>().center();
    pathfinder->setGoal(playerCenter);

    Vector2D myCenter = transform->center();
    Vector2D steerTarget = pathfinder->hasPath() ? pathfinder->nextWaypoint()
                                                 : playerCenter;

    float dx = steerTarget.x - myCenter.x;
    float dy = steerTarget.y - myCenter.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist > 0.0001f) {
        transform->velocity.x = (dx / dist) * speed;
        transform->velocity.y = (dy / dist) * speed;
        sprite->play("moving");
    } else {
        transform->velocity.x = 0;
        transform->velocity.y = 0;
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

    bool justCaughtFire = !onFireLastFrame;
    onFireLastFrame = true;

    if (justCaughtFire || currentTime - lastFireDirectionChange > fireDirectionInterval) {
        float angle = (rand() % 360) * M_PI / 180.0f;
        fireDirection.x = std::cos(angle);
        fireDirection.y = std::sin(angle);
        lastFireDirectionChange = currentTime;
        fireDirectionInterval = 500 + rand() % 1001; 
    }
    
    // Move faster when on fire (2.5x speed)
    float fireSpeed = 2.5;
    transform->velocity.x = fireDirection.x * fireSpeed;
    transform->velocity.y = fireDirection.y * fireSpeed;
    
    // Rotate the enemy to face the movement direction
    transform->rotation = Math2D::angleBetweenPoints({0, 0}, fireDirection);
    
    sprite->play("moving");
}
