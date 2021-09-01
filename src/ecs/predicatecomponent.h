#ifndef PREDICATECOMPONENT_H
#define PREDICATECOMPONENT_H

#include <time.h>
#include <functional>
#include <SDL.h>
#include "ecs.h"

class PredicateComponent : public Component
{
public:

    PredicateComponent(std::function<bool()> p):
        pred(p)
    {
        consequence = [](){ return false; };
    }

    PredicateComponent& then(std::function<void()> p)
    {
        consequence = p;
        return *this;
    }

    void update() override
    {
        if(pred()){
            consequence();
            entity->setActive(false);
        }
    }


private:

    std::function<bool ()> pred;
    std::function<void ()> consequence;

};

#endif // PREDICATECOMPONENT_H
