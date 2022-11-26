#include "projectilecomponent.h"
#include "game.h"
#include "../collision.h"



ProjectileComponent& ProjectileComponent::setDamage(int value)
{
    damage = value;
    return *this;
}

ProjectileComponent &ProjectileComponent::setRange(int value)
{
    range = value;
    return *this;
}

ProjectileComponent &ProjectileComponent::setSize(int w, int h)
{
    transform->width = w;
    transform->height = h;
    collider->collider.w = w;
    collider->collider.h = h;
    return *this;
}

void ProjectileComponent::init()
{
    if(!entity->hasComponent<TransformComponent>())
        entity->addComponent<TransformComponent>();
    if(!entity->hasComponent<ColliderComponent>()){
        entity->addComponent<ColliderComponent>().onCollision([&](Entity& enemy)
        {
            if(enemy.hasComponent<DamageModelComponent>()) {
                DamageModelComponent& enemyDamage = enemy.getComponent<DamageModelComponent>();
                ProjectileComponent& pc = entity->getComponent<ProjectileComponent>();

                enemyDamage.health -= pc.damage;
                range-=enemyDamage.projectileRangeLoss;
                pc.hit = true;
            }

        });
    }

    transform = &entity->getComponent<TransformComponent>();
    if(!entity->hasComponent<ColliderComponent>())
        entity->addComponent<ColliderComponent>();
    collider = &entity->getComponent<ColliderComponent>();




    double radAngle = Math2D::deg2rad(Math2D::angleBetweenPoints(src, target));
    transform->pos.x = src.x;
    transform->pos.y = src.y;
    transform->width = width;
    transform->height = height;
    collider->collider.w = width;
    collider->collider.h = height;
    transform->velocity.y = m_speed * static_cast<float>(std::sin(radAngle));
    transform->velocity.x = m_speed * static_cast<float>(std::cos(radAngle));

    // Trace

    entity->addGroup(groupProjectiles);
}

void ProjectileComponent::spawnTrace()
{
    auto& e = entity->m_manager.addEntity();
    e.addComponent<DecayComponent>(150);
    e.addComponent<TransformComponent>(transform->pos.x, transform->pos.y, transform->width, transform->height);
    e.addComponent<SpriteComponent>("assets/projectile.png");
    e.addGroup(groupProjectiles);
}

void ProjectileComponent::update()
{

    auto distance = Math2D::distanceBetweenPoints(transform->pos, src);
    if(distance > range){
        entity->setActive(false);
    }

}
