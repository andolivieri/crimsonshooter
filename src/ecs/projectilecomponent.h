#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H

#include "ecs.h"
#include "transformcomponent.h"
#include "vector2d.h"
#include "spritecomponent.h"
#include "math2d.h"

class ProjectileComponent : public Component
{
private:
    TransformComponent* transform;
    SpriteComponent* sprite;

public:
    ProjectileComponent() = default;
    ProjectileComponent(const Vector2D& s, const Vector2D& t):
        src(s), target(t)
    {

    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();

        double radAngle = Math2D::deg2rad(Math2D::angleBetweenPoints(src, target));
        transform->speed = m_speed;
        transform->velocity.y = m_speed * static_cast<float>(std::sin(radAngle));
        transform->velocity.x = m_speed * static_cast<float>(std::cos(radAngle));
    }

    void update() override
    {

        auto distance = Math2D::distanceBetweenPoints(transform->pos, src);
        if(distance > range){
            std::cout << "Bullet out of range" << std::endl;
            entity->setActive(false);
        }

    }

    void draw() override
    {
        SDL_Rect r;
        r.x = transform->pos.x;
        r.y = transform->pos.y;
        r.w = 10;
        r.h = 10;
        SDL_RenderDrawRect(TextureManager::renderer, &r);
    }


    Vector2D src;
    Vector2D target;
    int range = 1000;

    int m_speed = 100;

};


#endif // WEAPONCOMPONENT_H
