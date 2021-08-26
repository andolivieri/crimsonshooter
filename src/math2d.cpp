#include "math2d.h"
#include <math.h>

double Math2D::angleBetweenPoints(const SDL_Point &p1, const SDL_Point &p2)
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

double Math2D::rad2deg(const double rad)
{
    return rad*180 / M_PI;
}

double Math2D::deg2rad(const double deg)
{
    return deg*M_PI / 180;
}

