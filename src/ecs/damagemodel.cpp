#include "damagemodel.h"

DamageModelComponent::DamageModelComponent(const int h)
{
    health = h;
    deceaseTime = 0;
}

void DamageModelComponent::init()
{
    lastHealth = health;
}

void DamageModelComponent::update()
{

    // Just died...register the time
    justDied = isDead() && lastHealth > 0;
    if(justDied)
        deceaseTime = time(NULL);
    lastHealth = health;

}

bool DamageModelComponent::diedNow()
{
    return justDied;
}
