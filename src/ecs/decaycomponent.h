#ifndef DECAYCOMPONENT_H
#define DECAYCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"

class DecayComponent : public Component
{
public:

    DecayComponent(int t, int d=5){ decayTime = t; dissolveIn = d;}

    void init() override;
    void update() override;

private:
    int dissolveIn;
    int decayTime;
    time_t startTime;

};
#endif // DECAYCOMPONENT_H
