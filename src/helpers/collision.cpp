#include "helpers/collision.h"



bool Collision::AABB(const SDL_Rect &A, const SDL_Rect &B)
{
    return SDL_HasIntersection(&A, &B);
}

bool Collision::AABB(const ColliderComponent &A, const ColliderComponent &B)
{
    return AABB(A.collider, B.collider);
}
