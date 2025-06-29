#include "damagemodel.h"


DamageModelComponent::DamageModelComponent(const int start)
{
    health = start;
    startHealth = start;
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
    if(justDied) {
        lastDamageTaken = lastHealth - health;
        deceaseTime = time(NULL);
    }
    lastHealth = health;

}

bool DamageModelComponent::diedNow()
{
    return justDied;
}

bool DamageModelComponent::wasCriticalHit()
{
    const auto wasCritical =  justDied && lastDamageTaken >= startHealth * 5;
    if(wasCritical)
        std::cout << "CRITICAL HIT: " << lastDamageTaken << std::endl;
    return wasCritical;
}
