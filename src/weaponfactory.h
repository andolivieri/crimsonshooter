#ifndef WEAPONFACTORY_H
#define WEAPONFACTORY_H

#include "ecs.h"
#include "vector2d.h"

class WeaponData
{
public:

    std::string sprite;
    int magazine;
    int range = 500;
    int rate;
    int dps;
    int reloadTimeMsec;
    Vector2D muzzlePos;


    bool automatic = false;
    // Stati: idle, shooting, reloading

    std::string animationIdle = "idle";
    std::string animationReload = "reload";
    std::string animationFire = "shoot";

    std::string soundShoot;
    std::string soundReload;
    std::string soundEndfire;


    std::string projectileSprite = "assets/projectile.png";
    int projectileSize = 4;
    int projectileGauges = 1;
    int projectileSpreadAngle = 0;
    int projectileSpeed = 1;


    void update()
    {

    }
};

class WeaponFactory
{
public:
    WeaponFactory(EntityManager& em)
        : manager(em)
    {}

    Entity& createWeaponEntity(const std::string& weapon);
private:

    EntityManager& manager;

    Entity& createShotgun(Entity& e);

    Entity& createUzi(Entity& e);
};

#endif // WEAPONFACTORY_H
