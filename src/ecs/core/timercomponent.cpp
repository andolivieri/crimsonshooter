#include "timercomponent.h"


TimerComponent &TimerComponent::onTrigger(std::function<void (Entity&)> p)
{
    m_onTrigger = p;
    return *this;
}

void TimerComponent::init()
{
    m_startTime = SDL_GetTicks();
}

void TimerComponent::update()
{
    if(SDL_GetTicks() - m_startTime >= m_timeoutMsec && !triggered)
    {
        if(m_onTrigger)
            m_onTrigger(*entity);

        triggered =  true;

        if(m_repeating){
            m_startTime = SDL_GetTicks();
            triggered = false;
        }
    }

}
