#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "ecs.h"

class PositionComponent : public Component
{
public:


    //~PositionComponent(){};

    int x;
    int y;

    void init() override
    {
        x = 0;
        y = 0;
    }

    void update() override
    {

    }

    void draw() override
    {

    }


};

#endif // POSITIONCOMPONENT_H
