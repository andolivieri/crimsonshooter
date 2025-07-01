#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "ecs.h"
#include "ecs/core/transformcomponent.h"
#include "ecs/core/spritecomponent.h"
#include "ecs/gameplay/damagemodel.h"
#include "ecs/core/relationshipcomponent.h"
#include "ecs/core/collidercomponent.h"
#include "ecs/core/decaycomponent.h"
#include "ecs/effects/bloodpatch.h"
#include "ecs/effects/firecomponent.h"
#include "math2d.h"

class AIComponent : public Component
{
private:
    TransformComponent* transform;
    SpriteComponent* sprite;
    DamageModelComponent* damage;
    RelationshipComponent* family;

public:
    AIComponent() = default;

    float speed = 0.5;


    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
        damage = &entity->getComponent<DamageModelComponent>();
        family = &entity->getComponent<RelationshipComponent>();
    }

    void update() override;

    void randomtarget();
    bool isOnFire();
    void updateFireBehavior();

    Vector2D randtarget;
    Vector2D fireDirection;
    uint32_t lastFireDirectionChange = 0;

};

#endif // AICOMPONENT_H
