#ifndef MATH2D_H
#define MATH2D_H

#include "SDL.h"

class Math2D
{
public:
    static double angleBetweenPoints(const SDL_Point& a, const SDL_Point& b);
    static double rad2deg(const double rad);
    static double deg2rad(const double deg);
};

#endif // MATH2D_H
