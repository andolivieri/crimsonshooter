#ifndef COLLISION_H
#define COLLISION_H
#include <SDL.h>
#include "ecs/core/collidercomponent.h"

class Collision
{
public:
    static bool AABB(const SDL_Rect& A, const SDL_Rect& B);
    static bool AABB(const ColliderComponent& A, const ColliderComponent& B);
};

#endif // COLLISION_H
