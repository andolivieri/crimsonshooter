#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "ecs.h"

class PositionComponent : public Component
{
public:
    int x;
    int y;

    void init() override
    {
        x = 0;
        y = 0;
    }

    void update()
    {
        x+= 1;
        y+= 1;
    }

};

#endif // COMPONENTS_H
