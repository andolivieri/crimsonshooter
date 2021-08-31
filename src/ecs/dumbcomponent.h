#ifndef DUMBCOMPONENT_H
#define DUMBCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"

class DumbComponent : public Component
{
public:

    DumbComponent(int t=1){ dummyValue = t;}

    void init() override;
    void update() override;
    void draw() override;


private:
    int dummyValue;
    time_t startTime;

};
#endif // DUMBCOMPONENT_H
