#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "ecs.h"
#include "vector2d.h"

class TransformComponent : public Component
{
public:

    Vector2D pos;

    void init() override
    {
        pos.x = 0;
        pos.y = 0;
    }

    void update() override
    {

    }

    void draw() override
    {

    }


};

#endif // POSITIONCOMPONENT_H
