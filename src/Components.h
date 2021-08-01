#ifndef COMPONENTS_H
#define COMPONENTS_H

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
        x+= 1;
        y+= 1;
    }

    void draw() override
    {

    }


};

#endif // COMPONENTS_H
