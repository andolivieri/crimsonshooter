#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <string>
#include <functional>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"

class ColliderComponent : public Component
{
public:
    int paddingX = 0;
    int paddingY = 0;
    float scale = 1;
    SDL_Rect collider;
    std::string tag;
    TransformComponent* transform;

    ColliderComponent() {}
    ColliderComponent(const std::string& t): tag(t){}
    ColliderComponent(const std::string& t, int paddingX, int paddingY, float scale);

    ColliderComponent& onCollision(std::function<void(Entity& target)> p);

    void init() override;
    void update() override;
    void draw() override;


private:

    std::function<void(Entity& target)> onCollisionCb;

};

#endif // COLLIDERCOMPONENT_H
