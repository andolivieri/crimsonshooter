#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "ecs.h"
#include "gameobject.h"
#include "positioncomponent.h"

class AIComponent : public Component
{
private:
    PositionComponent* m_position;
    const GameObject& m_player;

public:
    AIComponent() = default;
    AIComponent(const GameObject& player):
        m_player(player)
    {

    }

    void init() override
    {
        m_position = &entity->getComponent<PositionComponent>();
    }

    void update() override
    {

            if(m_position->x > m_player.x)
                m_position->x--;
            else
                m_position->x++;

            if(m_position->y > m_player.y)
                m_position->y--;
            else
                m_position->y++;

    }

    void draw() override {}

};

#endif // AICOMPONENT_H
