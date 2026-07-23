#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include "ecs/ecs.h"
#include "ecs/base/transformcomponent.h"
#include "helpers/vector2d.h"
#include "ecs/base/spritecomponent.h"
#include "ecs/base/collidercomponent.h"
#include "helpers/math2d.h"
#include <functional>

class ProjectileComponent : public Component
{
private:
    TransformComponent* transform;
    ColliderComponent* collider;

    // invoked when the bullet stops because it reached its range end
    std::function<void(const Vector2D&)> m_onRangeEnd;

    void spawnTrace();
public:
    ProjectileComponent(const Vector2D& s, const Vector2D& t):
        src(s),
        target(t)
    {

    }

    ProjectileComponent& setDamage(int value);
    ProjectileComponent& setRange(int value);
    ProjectileComponent& setSpeed(float value);
    ProjectileComponent& setSize(int w, int h);
    ProjectileComponent& onRangeEnd(std::function<void(const Vector2D&)> cb);

    void init() override;
    void update() override;
    void draw() override;

    ProjectileComponent& setColor(SDL_Color c);

    Vector2D src;
    Vector2D target;
    int range = 1000;

    float m_speed = 30;
    int damage = 10;
    int width = 4;
    int height = 4;
    bool hit = false;

    // Rendered as an oriented streak in raw SDL (no sprite).
    SDL_Color color = {211, 173, 85, 255};
    int renderLength = 6;
    int renderThickness = 2;

};


#endif // PROJECTILECOMPONENT_H
