#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "ecs.h"
#include "transformcomponent.h"
#include "spritecomponent.h"
#include "damagemodel.h"
#include "relationshipcomponent.h"
#include "math2d.h"

class AIComponent : public Component
{
private:
    TransformComponent* transform;
    SpriteComponent* sprite;
    DamageModelComponent* damage;
    RelationshipComponent* family;

public:
    float speed = 0.5;
    bool victory = false;
    AIComponent() = default;


    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
        damage = &entity->getComponent<DamageModelComponent>();
        family = &entity->getComponent<RelationshipComponent>();
    }

    void update() override;

    void randomtarget();

    Vector2D randtarget;

};

#endif // AICOMPONENT_H
