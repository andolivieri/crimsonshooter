#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

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
    ColliderComponent* collider;

    void spawnTrace();
public:
    ProjectileComponent(const Vector2D& s, const Vector2D& t):
        src(s),
        target(t)
    {

    }

    ProjectileComponent& setDamage(int value);
    ProjectileComponent& setRange(int value);
    ProjectileComponent& setSize(int w, int h);

    void init() override;
    void update() override;

    Vector2D src;
    Vector2D target;
    int range = 1000;

    float m_speed = 30;
    int damage = 10;
    int width = 4;
    int height = 4;
    bool hit = false;

};


#endif // PROJECTILECOMPONENT_H
