#ifndef COLLISION_H
#define COLLISION_H
#include <SDL.h>
#include "ecs/base/collidercomponent.h"
#include "helpers/vector2d.h"

class Collision
{
public:
    static bool AABB(const SDL_Rect& A, const SDL_Rect& B);
    static bool AABB(const ColliderComponent& A, const ColliderComponent& B);

    // also outputs the minimum push-out vector
    static bool circleVsRect(const Vector2D& c, float radius,
                             const SDL_Rect& r, Vector2D& outPush);
};

#endif // COLLISION_H
