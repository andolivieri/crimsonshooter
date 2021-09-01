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
            entity->addComponent<DecayComponent>(decaytime);
            family->getChildren("shadow")->setActive(false);

        }
        return;
    }


    auto& target_pos =  player->getComponent<TransformComponent>();

    if(transform->pos.x > target_pos.pos.x)
        transform->velocity.x = -speed;
    else if (transform->pos.x < target_pos.pos.x)
        transform->velocity.x = speed;
    else
        transform->velocity.x = 0;

    if(transform->pos.y > target_pos.pos.y)
        transform->velocity.y = -speed;
    else if (transform->pos.y < target_pos.pos.y)
        transform->velocity.y = speed;
    else
        transform->velocity.y = 0;

    if(transform->velocity.x || transform->velocity.y )
    {
        sprite->play("moving");
    }else{
        sprite->play("idle");
    }

    transform->rotation = Math2D::angleBetweenPoints(transform->pos, target_pos.pos);

}
