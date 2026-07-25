#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H
#include "projectilecomponent.h"
#include "ecs/input/inputcomponent.h"
#include "engine/assetmanager.h"
#include "helpers/weaponfactory.h"
#include "ecs/base/fsmcomponent.h"

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

    Vector2D getAttachMargin();
    WeaponData& wpData();

    void triggerPull();
    void triggerRelease();
    void reload();

    int currentMagazineShotCount = 0;
    

    // Control intents consumed by the FSM states.
    bool triggerDown = false;
    bool reloadRequested = false;
private:

    WeaponData weapondata;
    int tracerCounter = 0;
    TransformComponent* transform;
    FSM_StateBase* state = nullptr;

};
#endif // WEAPONCOMPONENT_H
