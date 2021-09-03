#include "weaponcomponent.h"



FSM_StateBase *WeaponStateIdle::handleInput()
{
    for(auto e : input->frameEvents)
    {
        // TRIGGER_PULL: => shooting
        if(e.button == BTN_FIRE_1 && e.evt == BTN_PRESS)
            return new WeaponStateShooting(weaponData, entity);
    }
    return this;
}

FSM_StateBase *WeaponStateShooting::handleInput()
{

    if(weapon->currentMagazineShotCount >= weaponData.magazine)
    {
        return new WeaponStateReloading(weaponData, entity);
    }

    // TRIGGER_RELEASE: => idle
    for(auto e : input->frameEvents)
    {
        // TRIGGER_PULL: => shooting
        if(e.button == BTN_FIRE_1 && e.evt == BTN_RELEASE)
            return new WeaponStateIdle(weaponData, entity);
    }


    if(SDL_GetTicks() - lastShot >= weaponData.rate)
    {
        // shot
        sound->play(weaponData.soundShoot, 0);
        weapon->createProjectiles();
        lastShot = SDL_GetTicks();
        weapon->currentMagazineShotCount++;
        std::cout << "SHOTS: " << weapon->currentMagazineShotCount << std::endl;
    }
    return this;
}

FSM_StateBase *WeaponStateReloading::handleInput()
{
    // no event accepted just wait for reload to complete
    if(SDL_GetTicks() - startTime > weaponData.reloadTimeMsec)
    {
        std::cout << "RELOADED" << std::endl;
        weapon->currentMagazineShotCount = 0;
        return new WeaponStateIdle(weaponData, entity);
    }
    return this;
}
