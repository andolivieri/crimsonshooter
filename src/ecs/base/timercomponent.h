#ifndef TIMERCOMPONENT_H
#define TIMERCOMPONENT_H

#include <stdint.h>
#include <functional>

#include <SDL.h>
#include "ecs/ecs.h"

class TimerComponent : public Component
{
public:

    TimerComponent(uint32_t msecs, bool repeat=false)
    {
        m_timeoutMsec = msecs;
        m_repeating = repeat;
    }

    TimerComponent& onTrigger(std::function<void(Entity&)> p);

    void init() override;
    void update() override;


private:
    uint32_t m_timeoutMsec;
    bool m_repeating;
    uint32_t m_startTime;
    bool triggered = false;

    std::function<void(Entity& )> m_onTrigger;

};
#endif // TIMERCOMPONENT_H
