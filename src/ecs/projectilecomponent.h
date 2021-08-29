#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H

#include "ecs.h"
#include "transformcomponent.h"
#include "vector2d.h"
#include "spritecomponent.h"
#include "collidercomponent.h"
#include "math2d.h"

class ProjectileComponent : public Component
{
private:
    TransformComponent* transform;
    SpriteComponent* sprite;
    ColliderComponent* collider;

public:
    ProjectileComponent() = default;
    ProjectileComponent(const Vector2D& s, const Vector2D& t):
        src(s), target(t)
    {

    }


    ProjectileComponent& setDamage(int value);
    ProjectileComponent& setSize(int w, int h);

    void init() override;

    void update() override
    {

        auto distance = Math2D::distanceBetweenPoints(transform->pos, src);
        if(distance > range){
            entity->setActive(false);
        }

    }

    Vector2D src;
    Vector2D target;
    int range = 1000;

    int m_speed = 30;
    int damage = 20;

};


#endif // WEAPONCOMPONENT_H
