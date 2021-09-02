#ifndef WEAPONFACTORY_H
#define WEAPONFACTORY_H

#include "ecs.h"
#include "ecs/animation.h"
#include "ecs/components.h"

class BaseWeapon
{

    enum {
        STATE_IDLE,
        STATE_SHOOTING,
        STATE_RELOADING
    } WeaponStates;

    std::string sprite;
    int magazine;
    int range;
    int rate;
    int dps;

    // Stati: idle, shooting, reloading

    Animation animationIdle;
    Animation animationReload;
    Animation animationFire;

    void update()
    {

    }
};

class WeaponFactory
{
public:
    WeaponFactory(EntityManager& em)
        : manager(em)
    {

    };

    Entity& createWeaponEntity(EntityManager& manager, const std::string& weapon)
    {
        auto& theweapon = manager.addEntity();
        if(weapon == "shotgun")
            createShotgun(theweapon);
        else
            createUzi(theweapon);
        theweapon.addGroup(groupWeapons);
        return theweapon;
    }
private:

    EntityManager& manager;

    Entity& createShotgun(Entity& e)
    {
        e.addComponent<TransformComponent>();
        e.getComponent<TransformComponent>().width = 64;
        e.getComponent<TransformComponent>().height = 64;
        e.addComponent<SpriteComponent>("assets/shotgun.png")
                .addAnimation("idle", {0, 1, 100 })
                .addAnimation("shooting", {0, 12, 100});
        e.addGroup(groupWeapons);
        return e;
    }

    Entity& createUzi(Entity& e)
    {
        e.addComponent<TransformComponent>();
        e.getComponent<TransformComponent>().width = 64;
        e.getComponent<TransformComponent>().height = 64;
        e.addComponent<SpriteComponent>("assets/uzi.png")
                .addAnimation("idle", {0, 1, 100 })
                .addAnimation("shooting", {0, 4, 50});
        e.addGroup(groupWeapons);
        return e;
    }
};

#endif // WEAPONFACTORY_H
