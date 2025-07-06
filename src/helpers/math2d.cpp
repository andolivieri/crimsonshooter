#include "helpers/math2d.h"
#include <math.h>
#include <cmath>

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
   return  std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}


double Math2D::rad2deg(const double rad)
{
    return rad*180 / M_PI;
}

double Math2D::deg2rad(const double deg)
{
    return deg*M_PI / 180;
}

Vector2D Math2D::randomAround(int radius, const Vector2D &a)
{
    Vector2D r = a;
    int m = rand() > RAND_MAX / 2 ? -1 : 1;
    r.x += (rand() % radius) * m;
    r.y += (rand() % radius) * m;
    return r;
}

Vector2D Math2D::rotate_point(const Vector2D& pivot,double degAngle,const Vector2D& point)
{
  Vector2D res = point;
  double angle = deg2rad(degAngle);
  double s = sin(angle);
  double c = cos(angle);

  // translate point back to origin:
  res.x -= pivot.x;
  res.y -= pivot.y;

  // rotate point
  double xnew = res.x * c - res.y * s;
  double ynew = res.x * s + res.y * c;

  // translate point back:
  res.x = static_cast<float>(xnew + pivot.x);
  res.y = static_cast<float>(ynew + pivot.y);
  return res;
}

