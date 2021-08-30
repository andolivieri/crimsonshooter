#include "damagemodel.h"

DamageModelComponent::DamageModelComponent(const int h)
{
    health = h;
    lastHealth = health;
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

    if(health <= 0 && time(NULL) - deceaseTime > decayTimeSeconds)
        entity->setActive(false);
}

bool DamageModelComponent::diedNow()
{
    return justDied;
}
