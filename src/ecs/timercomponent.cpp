#include "timercomponent.h"


void TimerComponent::init()
{
    m_startTime = SDL_GetTicks();
}

void TimerComponent::update()
{
    if(SDL_GetTicks() - m_startTime >= m_timeoutMsec )
    {
        // fired

        if(m_repeating)
            m_startTime = SDL_GetTicks();
    }

}
