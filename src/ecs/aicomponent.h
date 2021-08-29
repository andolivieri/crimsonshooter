#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "ecs.h"
#include "transformcomponent.h"

class AIComponent : public Component
{
private:
    TransformComponent* m_position;
    const Entity& m_target;

public:
    AIComponent() = default;
    AIComponent(const Entity& player):
        m_target(player)
    {

    }

    void init() override
    {
        m_position = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {

            float speed = 0.5;
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

    }

};

#endif // AICOMPONENT_H
