#include "collidercomponent.h"
#include "game.h"

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
    transform = &entity->getComponent<TransformComponent>();

}

void ColliderComponent::update()
{
    collider.x = (int)transform->pos.x + paddingX;
    collider.y = (int)transform->pos.y + paddingY;
    collider.w = transform->width * scale;
    collider.h = transform->height  * scale;
}

void ColliderComponent::draw()
{

#ifdef ___DEBUG
    SDL_RenderDrawRect(TextureManager::renderer, &collider);
#endif
}
