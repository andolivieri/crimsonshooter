#ifndef DECAYCOMPONENT_H
#define DECAYCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"

class DecayComponent : public Component
{
public:

    DecayComponent(int t, int d=3000):
        decayTimeMsec(t),
        dissolveInMsec(d) {}

    void init() override;
    void update() override;

private:
    Uint32 dissolveInMsec;
    Uint32 decayTimeMsec;
    Uint32 startTimeMsec;

};
#endif // DECAYCOMPONENT_H
