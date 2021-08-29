#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "ecs.h"
#include "transformcomponent.h"
#include "spritecomponent.h"
#include "math2d.h"

class AIComponent : public Component
{
private:
    TransformComponent* m_position;
    SpriteComponent* sprite;
    const Entity& m_target;

public:
    float speed = 0.5;
    AIComponent() = default;
    AIComponent(const Entity& player):
        m_target(player)
    {

    }

    void init() override
    {
        m_position = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {

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

            if(m_position->velocity.x > 0 || m_position->velocity.y > 0)
            {
                sprite->play("moving");
            }else{
                sprite->play("idle");
            }

            m_position->rotation = Math2D::angleBetweenPoints(m_position->pos, m_player->pos);

    }

};

#endif // AICOMPONENT_H
