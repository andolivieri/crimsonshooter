#include "math2d.h"
#include <math.h>

double Math2D::angleBetweenPoints(const SDL_Point &p1, const SDL_Point &p2)
{
    return atan2(p2.y - p1.y, p2.x - p1.x) * 180 / M_PI;
}

double Math2D::angleBetweenPoints(const Vector2D &p1, const Vector2D &p2)
{
    //Angle returned as:
    //                      90
    //            135                45
    //
    //       180          Origin           0
    //
    //           -135                -45
    //
    //                     -90

    return atan2(p2.y - p1.y, p2.x - p1.x) * 180 / M_PI;
}

double Math2D::distanceBetweenPoints(const Vector2D &a, const Vector2D &b)
{
   return  std::sqrtf(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}


double Math2D::rad2deg(const double rad)
{
    return rad*180 / M_PI;
}

double Math2D::deg2rad(const double deg)
{
    return deg*M_PI / 180;
}

Vector2D &Math2D::randomAround(int radius, const Vector2D &a)
{
    Vector2D r = a;
    int m = rand() > RAND_MAX / 2 ? -1 : 1;
    r.x += (rand() % radius) * m;
    r.y += (rand() % radius) * m;
    return r;
}

