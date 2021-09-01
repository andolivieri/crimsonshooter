#ifndef PREDICATECOMPONENT_H
#define PREDICATECOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"

class PredicateComponent : public Component
{
public:

    PredicateComponent()
    {

    }

    void update() override;


private:


};

#endif // PREDICATECOMPONENT_H
