#include "aicomponent.h"
#include "game.h"

void AIComponent::update()
{

    if(damage->isDead()){
        sprite->play("dying", 1);
        transform->velocity.x = 0;
        transform->velocity.y = 0;
        entity->delGroup(groupEnemies);
        entity->addGroup(groupDeadEnemies);
        if(damage->diedNow()){
            std::cout << "Adding bloodpatch" << std::endl;
            auto& e = entity->m_manager.addEntity();
            e.addComponent<BloodPatchComponent>(transform->center());
        }
        return;
    }


    TransformComponent* m_player =  &m_target.getComponent<TransformComponent>();

    if(transform->pos.x > m_player->pos.x)
        transform->velocity.x = -speed;
    else if (transform->pos.x < m_player->pos.x)
        transform->velocity.x = speed;
    else
        transform->velocity.x = 0;

    if(transform->pos.y > m_player->pos.y)
        transform->velocity.y = -speed;
    else if (transform->pos.y < m_player->pos.y)
        transform->velocity.y = speed;
    else
        transform->velocity.y = 0;

    if(transform->velocity.x || transform->velocity.y )
    {
        sprite->play("moving");
    }else{
        sprite->play("idle");
    }

    transform->rotation = Math2D::angleBetweenPoints(transform->pos, m_player->pos);

}
