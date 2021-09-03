#include "weaponfactory.h"
#include "game.h"
#include "ecs/animation.h"
#include "ecs/components.h"
#include "ecs/weaponcomponent.h"


Entity &WeaponFactory::createWeaponEntity(const std::string &weapon)
{
    auto& theweapon = manager.addEntity();
    if(weapon == "shotgun")
        createShotgun(theweapon);
    else
        createUzi(theweapon);
    theweapon.addGroup(groupWeapons);
    return theweapon;
}

Entity &WeaponFactory::createShotgun(Entity &e)
{

    WeaponData wp;
    wp.animationFire = "shooting";
    wp.animationIdle = "idle";
    wp.animationReload = "reload";
    wp.sprite = "assets/shotgun.png";
    wp.automatic = false;
    wp.range = 100;
    wp.muzzlePos.x = 40;
    wp.rate = 1;
    wp.magazine = 1;
    wp.reloadTimeMsec = 600;

    wp.soundShoot = "assets/sounds/shotgun_shoot.wav";



    e.addComponent<TransformComponent>();
    e.getComponent<TransformComponent>().width = 64;
    e.getComponent<TransformComponent>().height = 64;
    e.addComponent<SpriteComponent>("assets/shotgun.png")
            .addAnimation("idle", {0, 0, 1, 100 })
            .addAnimation("shoot", {0, 0, 4, 100})
            .addAnimation("reload", {4, 0, 4, 150});
    e.addComponent<SoundComponent>();
    e.addComponent<InputComponent>();
    e.addComponent<WeaponComponent>(wp);
    e.addGroup(groupWeapons);
    return e;
}

Entity &WeaponFactory::createUzi(Entity &e)
{
    e.addComponent<TransformComponent>();
    e.getComponent<TransformComponent>().width = 64;
    e.getComponent<TransformComponent>().height = 64;
    e.addComponent<SpriteComponent>("assets/uzi.png")
            .addAnimation("idle", {0, 0, 1, 100 })
            .addAnimation("shooting", {0, 0, 4, 50});
    e.addComponent<SoundComponent>();
    e.addGroup(groupWeapons);
    return e;
}
