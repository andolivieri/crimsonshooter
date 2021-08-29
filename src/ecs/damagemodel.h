#ifndef DAMAGEMODEL_H
#define DAMAGEMODEL_H

#include <string>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"

class DamageModelComponent : public Component
{
public:

    void update() override;

    int health = 100;

    int projectileSpeedLoss = 10;

};

#endif // DAMAGEMODEL_H
