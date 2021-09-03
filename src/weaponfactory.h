#ifndef WEAPONFACTORY_H
#define WEAPONFACTORY_H

#include "ecs.h"
#include "ecs/animation.h"
#include "ecs/components.h"

class BaseWeapon
{
public:

    std::string sprite;
    int magazine;
    int range;
    int rate;
    int dps;
    int reloadTimeMsec;
    Vector2D muzzlePos;


    bool automatic = false;
    // Stati: idle, shooting, reloading

    std::string animationIdle = "idle";
    std::string animationReload = "reload";
    std::string animationFire = "shoot";


    std::string soundShoot = "shoot";
    std::string soundReload = "reload";
    std::string soundEndfire = "bullets";

    void update()
    {

    }
};

class WeaponStateBase : public FSM_StateBase
{
public:
    SpriteComponent* sprite;
    SoundComponent* sound;
    BaseWeapon* weapon;
};

class WeaponStateIdle : public WeaponStateBase
{

    void onEnter()
    {
        sprite->play("idle");
    }

    FSM_StateBase* handleInput()
    {
        // TODO TRIGGER_PULL: => shooting
    }
};

class WeaponStateShooting : public WeaponStateBase
{
    void onEnter()
    {
        sprite->play(weapon->animationFire);
        int loops = weapon->automatic ? -1 : 0;
        sound->play(weapon->soundShoot, loops);
    }

    FSM_StateBase* handleInput()
    {
        // TRIGGER_RELEASE: => idle
    }

    void onExit()
    {
        sprite->play("bullets");
    }
};

class WeaponStateReloading :  public WeaponStateBase
{

    void onEnter()
    {
        sprite->play("reload");
        sound->play("reload");
    }

    FSM_StateBase* handleInput()
    {
        // until reloadTimeMsec => idle
    }
};


class WeaponFactory
{
public:
    WeaponFactory(EntityManager& em)
        : manager(em)
    {

    };

    Entity& createWeaponEntity(const std::string& weapon)
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
        e.addComponent<SoundComponent>();
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
        e.addComponent<SoundComponent>();
        e.addGroup(groupWeapons);
        return e;
    }
};

#endif // WEAPONFACTORY_H
