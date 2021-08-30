#include "aicomponent.h"
#include "game.h"

void AIComponent::update()
{

    if(damage->isDead()){
        sprite->play("dying", 1);
        m_position->velocity.x = 0;
        m_position->velocity.y = 0;
        entity->delGroup(groupEnemies);
        entity->addGroup(groupDeadEnemies);
        return;
    }


    TransformComponent* m_player =  &m_target.getComponent<TransformComponent>();

    if(m_position->pos.x > m_player->pos.x)
        m_position->velocity.x = -speed;
    else if (m_position->pos.x < m_player->pos.x)
        m_position->velocity.x = speed;
    else
        m_position->velocity.x = 0;

    if(m_position->pos.y > m_player->pos.y)
        m_position->velocity.y = -speed;
    else if (m_position->pos.y < m_player->pos.y)
        m_position->velocity.y = speed;
    else
        m_position->velocity.y = 0;

    if(m_position->velocity.x || m_position->velocity.y )
    {
        sprite->play("moving");
    }else{
        sprite->play("idle");
    }

    m_position->rotation = Math2D::angleBetweenPoints(m_position->pos, m_player->pos);

}
