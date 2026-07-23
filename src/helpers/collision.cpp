#include "helpers/collision.h"
#include <algorithm>
#include <cmath>



bool Collision::AABB(const SDL_Rect &A, const SDL_Rect &B)
{
    return SDL_HasIntersection(&A, &B);
}

bool Collision::AABB(const ColliderComponent &A, const ColliderComponent &B)
{
    return AABB(A.collider, B.collider);
}

bool Collision::circleVsRect(const Vector2D &c, float radius,
                             const SDL_Rect &r, Vector2D &outPush)
{
    const float left = static_cast<float>(r.x);
    const float top = static_cast<float>(r.y);
    const float right = static_cast<float>(r.x + r.w);
    const float bottom = static_cast<float>(r.y + r.h);

    // closest point on the rect to the circle center
    const float px = std::max(left, std::min(c.x, right));
    const float py = std::max(top, std::min(c.y, bottom));

    const float dx = c.x - px;
    const float dy = c.y - py;
    const float distSq = dx * dx + dy * dy;

    if (distSq >= radius * radius) {
        return false;
    }

    if (distSq > 0.0001f) {
        // center outside the rect: push along the true contact normal
        const float dist = std::sqrt(distSq);
        const float push = radius - dist;
        outPush.x = (dx / dist) * push;
        outPush.y = (dy / dist) * push;
    } else {
        // center inside the rect: eject along the smallest-overlap axis
        const float toLeft = c.x - left;
        const float toRight = right - c.x;
        const float toTop = c.y - top;
        const float toBottom = bottom - c.y;

        const float minX = std::min(toLeft, toRight);
        const float minY = std::min(toTop, toBottom);

        if (minX < minY) {
            outPush.x = (toLeft < toRight ? -(minX + radius) : (minX + radius));
            outPush.y = 0.0f;
        } else {
            outPush.x = 0.0f;
            outPush.y = (toTop < toBottom ? -(minY + radius) : (minY + radius));
        }
    }
    return true;
}
