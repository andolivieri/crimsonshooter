#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "ecs.h"
#include "transformcomponent.h"
#include "spritecomponent.h"
#include "damagemodel.h"
#include "math2d.h"

class AIComponent : public Component
{
private:
    TransformComponent* m_position;
    SpriteComponent* sprite;
    DamageModelComponent* damage;
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
        damage = &entity->getComponent<DamageModelComponent>();
    }

    void update() override;

};

#endif // AICOMPONENT_H
