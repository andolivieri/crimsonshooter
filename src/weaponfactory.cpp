#include "weaponfactory.h"
#include "game.h"
#include "ecs/animation.h"
#include "ecs/components.h"
#include "ecs/weaponcomponent.h"


Entity &WeaponFactory::createWeaponEntity(const std::string &weapon, bool mirrored)
{
    auto& theweapon = manager.addEntity();
    if(weapon == "shotgun")
        createShotgun(theweapon);
    else if(weapon == "uzi")
        createUzi(theweapon);
    else
        createHandgun(theweapon);
    theweapon.addGroup(groupWeapons);
    if(mirrored)
        theweapon.getComponent<SpriteComponent>().flip =SDL_FLIP_VERTICAL;
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
    wp.range = 500;
    wp.muzzlePos.x = 40;
    wp.rate = 300;
    wp.magazine = 1;
    wp.reloadTimeMsec = 1200;

    wp.soundShoot = "assets/sounds/shotgun_shoot.wav";
    wp.projectileGauges = 12;
    wp.projectileSpreadAngle = 25;
    wp.projectileDamage = 30;


    e.addComponent<TransformComponent>();
    e.getComponent<TransformComponent>().width = 64;
    e.getComponent<TransformComponent>().height = 64;
    e.addComponent<SpriteComponent>("assets/shotgun.png")
            .addAnimation("idle", {0, 0, 1, 100 })
            .addAnimation("shoot", {0, 0, 4, 100})
            .addAnimation("reload", {4, 0, 8, 150});
    e.addComponent<SoundComponent>();
    e.addComponent<InputComponent>();
    e.addComponent<WeaponComponent>(wp);
    e.addGroup(groupWeapons);
    return e;
}

Entity &WeaponFactory::createUzi(Entity &e)
{

    WeaponData wp;
    wp.animationFire = "shooting";
    wp.animationIdle = "idle";
    wp.animationReload = "reload";
    wp.sprite = "assets/uzi.png";
    wp.automatic = true;
    wp.range = 800;
    wp.muzzlePos.x = 25;
    wp.muzzlePos.y = -6;
    wp.rate = 50;
    wp.magazine = 35;
    wp.reloadTimeMsec = 2000;


    wp.soundShoot = "assets/sounds/uzi_shoot.wav";
    wp.soundEndfire = "assets/sounds/uzi_endshoot.wav";
    wp.soundReload = "assets/sounds/uzi_reload.wav";
    wp.projectileGauges = 1;
    wp.projectileSize = 4;
    wp.projectileSpreadAngle = 7;
    wp.projectileSpeed = 100;

    e.addComponent<TransformComponent>();
    e.getComponent<TransformComponent>().width = 64;
    e.getComponent<TransformComponent>().height = 64;
    e.addComponent<SpriteComponent>("assets/uzi.png")
            .addAnimation("idle", {0, 0, 1, 100 })
            .addAnimation("shoot", {0, 0, 4, 20})
            .addAnimation("reload", {4, 0, 11, 100});
    e.addComponent<SoundComponent>();
    e.addComponent<InputComponent>();
    e.addComponent<WeaponComponent>(wp);
    e.addGroup(groupWeapons);
    return e;
}

Entity &WeaponFactory::createHandgun(Entity &e)
{

    WeaponData wp;
    wp.animationFire = "shooting";
    wp.animationIdle = "idle";
    wp.animationReload = "reload";
    wp.sprite = "assets/handgun.png";
    wp.automatic = false;
    wp.range = 800;
    wp.muzzlePos.x = 25;
    wp.muzzlePos.y = -6;
    wp.rate = 800;
    wp.magazine = 12;
    wp.reloadTimeMsec = 2000;


    wp.soundShoot = "assets/sounds/handgun_shoot.wav";
    wp.soundReload = "assets/sounds/handgun_reload.wav";
    wp.projectileGauges = 1;
    wp.projectileSize = 2;
    wp.projectileSpreadAngle = 2;
    wp.projectileSpeed = 100;

    e.addComponent<TransformComponent>();
    e.getComponent<TransformComponent>().width = 64;
    e.getComponent<TransformComponent>().height = 64;
    e.addComponent<SpriteComponent>("assets/handgun.png")
            .addAnimation("idle", {0, 0, 1, 100 })
            .addAnimation("shoot", {0, 0, 4, 20})
            .addAnimation("reload", {0, 0, 1, 100});
    e.addComponent<SoundComponent>();
    e.addComponent<InputComponent>();
    e.addComponent<WeaponComponent>(wp);
    e.addGroup(groupWeapons);
    return e;
}
