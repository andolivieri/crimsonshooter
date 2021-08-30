#include "damagemodel.h"

void DamageModelComponent::update()
{

    // Just died...register the time
    if(isDead() && lastHealth > 0)
        deceaseTime = time(NULL);
    lastHealth = health;

    if(health <= 0 && time(NULL) - deceaseTime > decayTimeSeconds)
        entity->setActive(false);
}
