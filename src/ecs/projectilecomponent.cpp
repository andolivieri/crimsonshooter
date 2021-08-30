#include "projectilecomponent.h"
#include "game.h"



ProjectileComponent& ProjectileComponent::setDamage(int value)
{
    damage = value;
    return *this;
}

ProjectileComponent &ProjectileComponent::setSize(int w, int h)
{
    collider->collider.w = w;
    collider->collider.h = h;
    return *this;
}

void ProjectileComponent::init()
{
    if(!entity->hasComponent<TransformComponent>())
        entity->addComponent<TransformComponent>();
    if(!entity->hasComponent<ColliderComponent>())
        entity->addComponent<ColliderComponent>();

    transform = &entity->getComponent<TransformComponent>();
    collider = &entity->addComponent<ColliderComponent>();
    collider->collider.w = 2;
    collider->collider.h = 2;
    transform->width = 8;
    transform->height = 8;



    double radAngle = Math2D::deg2rad(Math2D::angleBetweenPoints(src, target));
    transform->speed = m_speed;
    transform->pos.x = src.x;
    transform->pos.y = src.y;
    transform->velocity.y = m_speed * static_cast<float>(std::sin(radAngle));
    transform->velocity.x = m_speed * static_cast<float>(std::cos(radAngle));

    entity->addGroup(groupProjectiles);
}
