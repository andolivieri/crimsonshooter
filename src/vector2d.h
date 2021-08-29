#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>


class Vector2D
{
public:

    float x;
    float y;

    Vector2D();
    Vector2D(float x, float y);
    Vector2D(int x, int y);

    Vector2D& add(const Vector2D& vec);
    Vector2D& subtract(const Vector2D& vec);
    Vector2D& multiply(const Vector2D& vec);
    Vector2D& divide(const Vector2D& vec);

    friend Vector2D& operator+(Vector2D& v1, const Vector2D&v2) {
        return v1.add(v2);
    }
    friend Vector2D& operator-(Vector2D& v1, const Vector2D&v2) {
        return v1.subtract(v2);
    }
    friend Vector2D& operator*(Vector2D& v1, const Vector2D&v2) {
        return v1.multiply(v2);
    }
    friend Vector2D& operator/(Vector2D& v1, const Vector2D&v2){
        return v1.divide(v2);
    }

    Vector2D& operator+=(const Vector2D& vec);
    Vector2D& operator-=(const Vector2D& vec);
    Vector2D& operator*=(const Vector2D& vec);
    Vector2D& operator/=(const Vector2D& vec);

    friend std::ostream& operator<<(std::ostream& s, const Vector2D& v)
    {
        s << "Vector2D(" << v.x << "," << v.y << ")";
        return s;
    }
};

#endif // VECTOR2D_H
