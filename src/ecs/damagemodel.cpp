#include "damagemodel.h"

void DamageModelComponent::update()
{
    if(health < 0)
        entity->setActive(false);
}
