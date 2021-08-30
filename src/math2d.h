#ifndef MATH2D_H
#define MATH2D_H

#include "SDL.h"
#include "vector2d.h"

class Math2D
{
public:
    static double angleBetweenPoints(const SDL_Point& a, const SDL_Point& b);
    static double angleBetweenPoints(const Vector2D& a, const Vector2D& b);
    static double distanceBetweenPoints(const Vector2D& a, const Vector2D& b);
    static double rad2deg(const double rad);
    static double deg2rad(const double deg);
    static Vector2D& randomAround(int radius, const Vector2D& a);
};

#endif // MATH2D_H
