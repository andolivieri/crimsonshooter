#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "ecs.h"
#include "vector2d.h"

class TransformComponent : public Component
{
public:

    Vector2D pos;
    Vector2D velocity;

    int speed = 1;
    int width = 32;
    int height = 32;
    int scale  = 1;

    TransformComponent()
    {
    }

    TransformComponent(const float x, const float y)
    {
        pos.x = x;
        pos.y = y;
        velocity.x = velocity.y = 0;
    }

    TransformComponent(const float x, const float y, const int w, const int h):
        TransformComponent(x,y)
    {
        width = w;
        height = h;
    }

    void update() override {
        pos.x += velocity.x * speed;
        pos.y += velocity.y * speed;
    }



};

#endif // POSITIONCOMPONENT_H
