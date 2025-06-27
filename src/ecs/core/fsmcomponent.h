#ifndef FSMCOMPONENT_H
#define FSMCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "ecs/input/inputcomponent.h"

class FSM_StateBase
{
public:
    virtual FSM_StateBase* handleInput(){return nullptr;};
    virtual void onEnter() {}
    virtual void onExit() {}
};

class FSMComponent : public Component
{
public:

    void update() override
    {
        if(state)
        {
            auto nextstate = state->handleInput();
            if(nextstate!=state)
            {
                state->onExit();
                nextstate->onEnter();
                delete state;
            }
            state = nextstate;
        }
    }

private:
    FSM_StateBase* state = nullptr;

};
#endif // FSMCOMPONENT_H
