#include "vector2d.h"


Vector2D::Vector2D()
{
    x = 0.f;
    y = 0.f;
}

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2D& Vector2D::add(const Vector2D &vec)
{
    this->x += vec.x;
    this->y += vec.y;

    return *this;
}

Vector2D& Vector2D::subtract(const Vector2D &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;

    return *this;
}

Vector2D& Vector2D::multiply(const Vector2D &vec)
{
    this->x *= vec.x;
    this->y *= vec.y;

    return *this;
}

Vector2D& Vector2D::divide(const Vector2D &vec)
{
    this->x /= vec.x;
    this->y /= vec.y;

    return *this;
}

Vector2D &Vector2D::operator+=(const Vector2D &vec)
{
    return add(vec);
}

Vector2D &Vector2D::operator-=(const Vector2D &vec)
{
    return subtract(vec);
}

Vector2D &Vector2D::operator*=(const Vector2D &vec)
{
    return multiply(vec);
}

Vector2D &Vector2D::operator/=(const Vector2D &vec)
{
    return divide(vec);
}




