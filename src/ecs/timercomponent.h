#ifndef TIMERCOMPONENT_H
#define TIMERCOMPONENT_H

#include <stdint.h>

#include <SDL.h>
#include "ecs.h"

class TimerComponent : public Component
{
public:

    TimerComponent(int msecs, bool repeat=false)
    {
        m_timeoutMsec = msecs;
        m_repeating = repeat;
    }

    void init() override;
    void update() override;


private:
    float m_timeoutMsec;
    bool m_repeating;
    uint32_t m_startTime;

};
#endif // TIMERCOMPONENT_H
