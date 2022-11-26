#ifndef DECAYCOMPONENT_H
#define DECAYCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"

class DecayComponent : public Component
{
public:

    DecayComponent(int t, int d=0):
        decayInMsec(t),
        fadeAfterMsec(d) {}

    void init() override;
    void update() override;

private:
    Uint32 fadeAfterMsec;

    Uint32 decayInMsec;
    Uint32 startTimeMsec;

};
#endif // DECAYCOMPONENT_H
