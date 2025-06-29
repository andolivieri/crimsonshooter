#ifndef DAMAGEMODEL_H
#define DAMAGEMODEL_H

#include <string>
#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "ecs/core/transformcomponent.h"

class DamageModelComponent : public Component
{
public:

    DamageModelComponent(const int start=100);

    void init() override;

    void update() override;

    bool isDead()
    {
        return health <= 0;
    }


    bool diedNow();

    bool wasCriticalHit();

    int health;
    int startHealth;
    int projectileRangeLoss = 500;

private:
    int lastHealth;
    bool justDied = false;
    int lastDamageTaken = 0;
    time_t deceaseTime;

};

#endif // DAMAGEMODEL_H
