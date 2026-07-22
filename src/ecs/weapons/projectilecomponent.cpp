#include "projectilecomponent.h"
#include "engine/game.h"
#include "helpers/collision.h"
#include "ecs/effects/bloodpatch.h"
#include "ecs/base/decaycomponent.h"
#include "helpers/math2d.h"
#include "helpers/utils.h"
#include "helpers/bloodspit.h"
#include <cmath>



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
        entity->addComponent<ColliderComponent>().onCollision([&](Entity& target)
        {
            if(target.getComponent<ColliderComponent>().isSolid){
                entity->destroy();
                return;
            }
            if(target.hasComponent<DamageModelComponent>() && target.tag != "player") {
                DamageModelComponent& enemyDamage = target.getComponent<DamageModelComponent>();
                ProjectileComponent& pc = entity->getComponent<ProjectileComponent>();

                enemyDamage.health -= pc.damage;
                range-=enemyDamage.projectileRangeLoss;
                pc.hit = true;
                
                // Create blood splat on projectile hit
                if(target.hasComponent<TransformComponent>()) {
                    auto& enemyTransform = target.getComponent<TransformComponent>();
                    auto& splat = entity->m_manager.addEntity();
                    int startSize = 4 + rand() % 8;
                    int endSize = startSize + 4 + rand() % 16;
                    splat.addComponent<BloodSplatComponent>(enemyTransform.center())
                            .setStartSize(startSize, startSize)
                            .setMaxSize(endSize, endSize);
                    splat.addComponent<DecayComponent>(ONE_MINUTE, ONE_MINUTE - 5*ONE_SECOND);

                    createBloodSpit(target);
                }
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
