#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <string>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"

class ColliderComponent : public Component
{
public:
    SDL_Rect collider;
    std::string tag;
    TransformComponent* transform;

    ColliderComponent() {}
    ColliderComponent(const std::string& t): tag(t){}

    void init() override;
    void update() override;

};

#endif // COLLIDERCOMPONENT_H
