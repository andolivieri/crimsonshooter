#include "collidercomponent.h"
#include "game.h"
#include "collision.h"

ColliderComponent::ColliderComponent(const std::string &t, int paddingX, int paddingY, float scale): tag(t)
{
    this->paddingX = paddingX;
    this->paddingY = paddingY;
    this->scale = scale;
}

void ColliderComponent::init()
{
    if(!entity->hasComponent<TransformComponent>())
    {
        entity->addComponent<TransformComponent>();
    }
    entity->addGroup(groupColliders);
    transform = &entity->getComponent<TransformComponent>();

}

void ColliderComponent::update()
{
    collider.x = (int)transform->pos.x + paddingX;
    collider.y = (int)transform->pos.y + paddingY;
    collider.w = static_cast<int>(transform->width * scale);
    collider.h = static_cast<int>(transform->height  * scale);

    if(onCollisionCb == nullptr){
        return;
    }

    auto& otherColliders = entity->m_manager.getGroup(groupColliders);

    for(auto& c : otherColliders){

        if(c  != entity && c->hasComponent<ColliderComponent>()){
            ColliderComponent& targetCollider = c->getComponent<ColliderComponent>();
            if(Collision::AABB(*this, targetCollider))
            {
               onCollisionCb(*c);
            }
        }

    }

}

void ColliderComponent::draw()
{

#if 0
    SDL_RenderDrawRect(TextureManager::renderer, &collider);
#endif
}

ColliderComponent& ColliderComponent::onCollision(std::function<void(Entity& target)> p)
{
    onCollisionCb = p;
    return *this;
}

