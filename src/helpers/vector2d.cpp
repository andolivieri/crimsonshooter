#include "helpers/vector2d.h"
#include <cmath>


Vector2D::Vector2D()
{
    x = 0.f;
    y = 0.f;
}

Vector2D::Vector2D(int x, int y)
{
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
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

float Vector2D::magnitude() const
{
    return sqrt(x * x + y * y);
}




