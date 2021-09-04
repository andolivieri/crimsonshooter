#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H
#include "projectilecomponent.h"
#include "tilecomponent.h"
#include "inputcomponent.h"
#include "assetmanager.h"
#include "weaponfactory.h"
#include "fsmcomponent.h"
#include "projectilecomponent.h"

class WeaponComponent : public Component
{
public:

    WeaponComponent(const WeaponData& w):
        weapondata(w)
    {
    }

    void init() override;

    void update() override;

    void createProjectiles();

    int currentMagazineShotCount = 0;

private:

    WeaponData weapondata;
    InputComponent* input;
    TransformComponent* transform;
    FSM_StateBase* state = nullptr;

};
#endif // WEAPONCOMPONENT_H
