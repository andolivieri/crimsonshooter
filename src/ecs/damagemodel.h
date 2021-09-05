#ifndef DAMAGEMODEL_H
#define DAMAGEMODEL_H

#include <string>
#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"

class DamageModelComponent : public Component
{
public:

    DamageModelComponent(){}
    DamageModelComponent(const int h);

    void init() override;

    void update() override;

    bool isDead()
    {
        return health <= 0;
    }


    bool diedNow();



    int health = 100;
    int projectileRangeLoss = 500;

private:
    int lastHealth;
    bool justDied = false;
    time_t deceaseTime;

};

#endif // DAMAGEMODEL_H
