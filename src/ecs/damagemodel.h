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
    DamageModelComponent(const int h)
    {
        health = h;
        lastHealth = health;
        deceaseTime = 0;
    }

    void init() override
    {
        lastHealth = health;
    }

    void update() override;

    bool isDead()
    {
        return health <= 0;
    }



    int health = 100;
    int projectileSpeedLoss = 10;
    int decayTimeSeconds = 60;

private:
    int lastHealth;
    time_t deceaseTime;

};

#endif // DAMAGEMODEL_H
