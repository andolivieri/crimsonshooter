#include "helpers/weaponfactory.h"
#include "engine/game.h"
#include "ecs/base/animation.h"
#include "ecs/components.h"
#include "ecs/weapons/weaponcomponent.h"
#include <iostream>
#include <cmath>
#include <climits>


Entity &WeaponFactory::createWeaponEntity(const std::string &weapon, bool mirrored)
{
    auto& theweapon = manager.addEntity();
    if(weapon == "shotgun")
        createShotgun(theweapon);
    else if(weapon == "uzi")
        createUzi(theweapon);
    else if(weapon == "mg")
        createMachineGun(theweapon);
    else if(weapon == "chainsaw")
        createChainsaw(theweapon);
    else
        createHandgun(theweapon);
    theweapon.addGroup(groupWeapons);
    if(mirrored)
        theweapon.getComponent<SpriteComponent>().flip =SDL_FLIP_VERTICAL;
    return theweapon;
}

Entity &WeaponFactory::createChainsaw(Entity &e)
{

    WeaponData wp;
    wp.sprite = "assets/chainsaw.png";
    wp.automatic = true;
    wp.range = 0;
    wp.rate = 50;
    wp.magazine = INT_MAX;
    wp.reloadTimeMsec = 0;

    wp.soundShoot = "assets/sounds/chainsaw_run.ogg";
    wp.soundEndfire =  "assets/sounds/chainsaw_release.ogg";
    wp.soundIdle =  "assets/sounds/chainsaw_idle.ogg";


    wp.attachMargin.x = 5;
    wp.attachMargin.y = 5;
    wp.chainsaw = true;


    e.addComponent<TransformComponent>();
    e.getComponent<TransformComponent>().width = 64;
    e.getComponent<TransformComponent>().height = 64;
    e.addComponent<SpriteComponent>("assets/chainsaw.png")
            .addAnimation("idle", {0, 0, 2, 100 })
            .addAnimation("reload", {0, 0, 1, 100 })
            .addAnimation("shoot", {2, 0, 2, 50});
    e.addComponent<SoundComponent>();
    e.addComponent<InputComponent>();
    e.addComponent<WeaponComponent>(wp);
    e.addComponent<ColliderComponent>().onCollision([&](Entity& target) {

        if(target.hasComponent<DamageModelComponent>() && &(target) != e.m_manager.get("player")){
            auto& dm = target.getComponent<DamageModelComponent>();
            dm.health -= 5;

            auto& bloodSpit = e.m_manager.addEntity();
            TransformComponent targetTt = target.getComponent<TransformComponent>();
            targetTt.velocity.x =  targetTt.velocity.y = 0;

            targetTt.rotation = rand() % 180;
            targetTt.pos.x +=  std::pow(-1, rand() % 3) * (rand() % targetTt.width/2);
            targetTt.pos.y +=  std::pow(-1, rand() % 3) * (rand() % targetTt.height/2);
            bloodSpit.addComponent<TransformComponent>(targetTt);
            const auto bf  = 10;
            bloodSpit.addComponent<SpriteComponent>("assets/blood.png")
                    .addAnimation("splat", {0, 0, 3 + (rand() % (bf-4)), 20 })
                    //.showFrame(true)
                    .play("splat", 1);
            bloodSpit.addGroup(groupBloodPatches);
            target.emplaceComponent<RelationshipComponent>().addChildren(&bloodSpit,"");


        }
    });
    e.addGroup(groupWeapons);
    return e;
}

Entity &WeaponFactory::createShotgun(Entity &e)
{

    WeaponData wp;
    wp.animationReload = "reload";
    wp.sprite = "assets/shotgun.png";
    wp.automatic = false;
    wp.range = 500;
    wp.muzzlePos.x = 40;
    wp.rate = 180;
    wp.magazine = 2;
    wp.reloadTimeMsec = 1200;
    
    wp.tracerEnabled = true;
    wp.tracerEvery = 1;
    wp.tracerLength = 6;
    wp.tracerColor = {124, 126, 123, 255};

    wp.soundShoot = "assets/sounds/shotgun_shoot.ogg";
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
    wp.sprite = "assets/uzi.png";
    wp.automatic = true;
    wp.range = 800;
    wp.muzzlePos.x = 25;
    wp.muzzlePos.y = -6;
    wp.rate = 800;
    wp.magazine = 32;
    wp.reloadTimeMsec = 2000;


    wp.soundShoot = "assets/sounds/uzi_shoot.ogg";
    wp.soundEndfire = "assets/sounds/uzi_endshoot.ogg";
    wp.soundReload = "assets/sounds/uzi_reload.ogg";
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
    wp.sprite = "assets/handgun.png";
    wp.automatic = false;
    wp.range = 800;
    wp.muzzlePos.x = 25;
    wp.muzzlePos.y = -6;
    wp.rate = 90;
    wp.magazine = 17;
    wp.reloadTimeMsec = 2000;

    
    wp.tracerEnabled = true;
    wp.tracerEvery = 1;
    wp.tracerLength = 6;
    wp.tracerColor = {124, 126, 123, 255};


    wp.soundShoot = "assets/sounds/handgun_shoot.ogg";
    wp.soundReload = "assets/sounds/handgun_reload.ogg";
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

Entity &WeaponFactory::createMachineGun(Entity &e)
{

    WeaponData wp;
    wp.sprite = "assets/uzi.png";
    wp.automatic = true;
    wp.range = 1500;
    wp.muzzlePos.x = 25;
    wp.muzzlePos.y = -6;
    wp.rate = 1700;
    wp.magazine = 250;
    wp.reloadTimeMsec = 5300;


    wp.soundShoot = "assets/sounds/mg_shoot.ogg";
    wp.soundEndfire = "assets/sounds/mg_endshoot.ogg";
    wp.soundReload = "assets/sounds/mg_reload.ogg";
    wp.projectileGauges = 1;
    wp.projectileSize = 4;
    wp.projectileSpreadAngle = 3;
    wp.projectileSpeed = 180;

    // green tracer, 1 bullet every 3
    wp.tracerEnabled = true;
    wp.tracerEvery = 3;
    wp.tracerColor = {80, 255, 80, 255};

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
