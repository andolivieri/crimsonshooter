#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "ecs.h"
#include "vector2d.h"

class TransformComponent : public Component
{
public:

    Vector2D pos;
    Vector2D velocity;

    float speed = 1;
    int width = 32;
    int height = 32;
    double rotation = 0;

    TransformComponent()
    {
    }

    TransformComponent(const float x, const float y)
    {
        pos.x = x;
        pos.y = y;
        velocity.x = velocity.y = 0;
    }

    TransformComponent(const Vector2D& v):
        TransformComponent(v.x, v.y)
    {
    }


    TransformComponent(const float x, const float y, const int w, const int h):
        TransformComponent(x,y)
    {
        width = w;
        height = h;
    }

    void update() override {
        pos.x += velocity.x;
        pos.y += velocity.y;
    }

    int scaledWidth() const {
        return width * scale;
    }

    int scaledHeight() const {
        return height * scale;
    }

    Vector2D center()
    {
        return Vector2D{pos.x + width/2, pos.y+height/2};
    }

private:

    int scale  = 1;



};

#endif // POSITIONCOMPONENT_H
