#include "dumbcomponent.h"


void DumbComponent::init()
{
    if(m_onInit)
        m_onInit();
}

void DumbComponent::update()
{
    if(m_onUpdate)
        m_onUpdate();
}

void DumbComponent::draw()
{
    if(m_onDraw)
        m_onDraw();
}

void DumbComponent::onDraw(std::function<void ()> f)
{
    m_onDraw = f;
}

void DumbComponent::onUpdate(std::function<void ()> f)
{
    m_onUpdate = f;
}


void DumbComponent::onInit(std::function<void ()> f)
{
    m_onInit = f;
}
