#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "ecs.h"
#include "gameobject.h"
#include "transformcomponent.h"

class AIComponent : public Component
{
private:
    TransformComponent* m_position;
    const GameObject& m_player;

public:
    AIComponent() = default;
    AIComponent(const GameObject& player):
        m_player(player)
    {

    }

    void init() override
    {
        m_position = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {

            if(m_position->pos.x > m_player.x)
                m_position->pos.x--;
            else
                m_position->pos.x++;

            if(m_position->pos.y > m_player.y)
                m_position->pos.y--;
            else
                m_position->pos.y++;

    }

    void draw() override {}

};

#endif // AICOMPONENT_H
