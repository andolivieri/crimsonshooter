#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <string>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"

class ColliderComponent : public Component
{
    SDL_Rect collider;
    std::string tag;

    TransformComponent* transform;

public:
    void init() override
    {
        if(!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        collider.x = (int)transform->pos.x;
        collider.y = (int)transform->pos.y;
        collider.w = transform->width * transform->scale;
        collider.h = transform->height  * transform->scale;
    }

};

#endif // COLLIDERCOMPONENT_H
