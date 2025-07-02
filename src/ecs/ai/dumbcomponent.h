#ifndef DUMBCOMPONENT_H
#define DUMBCOMPONENT_H

#include <functional>
#include <time.h>
#include <SDL.h>
#include "ecs/ecs.h"
#include "ecs/input/inputcomponent.h"

class DumbComponent : public Component
{
public:

    DumbComponent(){}

    void init() override;
    void update() override;
    void draw() override;

    void onDraw(std::function<void()> f);
    void onUpdate(std::function<void()> f);
    void onInit(std::function<void()> f);


private:

    std::function<void ()> m_onDraw;
    std::function<void ()> m_onUpdate;
    std::function<void ()> m_onInit;

};
#endif // DUMBCOMPONENT_H
