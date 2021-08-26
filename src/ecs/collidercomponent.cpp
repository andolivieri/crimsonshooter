#include "collidercomponent.h"
#include "game.h"

void ColliderComponent::init()
{
    if(!entity->hasComponent<TransformComponent>())
    {
        entity->addComponent<TransformComponent>();
    }
    transform = &entity->getComponent<TransformComponent>();
    Game::colliders.push_back(this);

}

void ColliderComponent::update()
{
    collider.x = (int)transform->pos.x;
    collider.y = (int)transform->pos.y;
    collider.w = transform->width * transform->scale;
    collider.h = transform->height  * transform->scale;
}
