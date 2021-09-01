#include "aicomponent.h"
#include "game.h"

void AIComponent::update()
{

    Entity* player = entity->m_manager.getGroup(groupPlayers)[0];

    if(damage->isDead()){

        int decaytime = 60*10;

        if(damage->diedNow()){
            sprite->play("dying", 1);
            transform->velocity.x = 0;
            transform->velocity.y = 0;
            entity->delGroup(groupEnemies);
            entity->addGroup(groupDeadEnemies);

            for(int i=0; i<3; i++){
                auto& splat = entity->m_manager.addEntity();
                int startSize = 8 + rand() % 16;
                int endsize = startSize + 8 +  rand() % 32;
                splat.addComponent<BloodSplatComponent>(Math2D::randomAround(8, transform->center()))
                        .setStartSize(startSize,startSize)
                        .setMaxSize(endsize, endsize);
                splat.addComponent<DecayComponent>(decaytime);
                family->addChildren(&splat, "splat_" + std::to_string(i));
            }


            family->getChildren("shadow")->setActive(false);
            entity->addComponent<DecayComponent>(decaytime);

        }
        return;
    }


    Vector2D target = player->getComponent<TransformComponent>().pos;
    /*
    if(player->getComponent<DamageModelComponent>().isDead())
    {
        target = player->getComponent<TransformComponent>().pos;
    }else{
        randomtarget();
        target = randtarget;
    }
    */

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
