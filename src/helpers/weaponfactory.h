#ifndef WEAPONFACTORY_H
#define WEAPONFACTORY_H

#include <cstdint>
#include <functional>
#include <SDL.h>
#include "ecs/ecs.h"
#include "helpers/vector2d.h"

class WeaponData
{
public:

    std::string sprite;
    int magazine;
    int range = 500;
    uint32_t rate;
    int dps;
    uint32_t reloadTimeMsec;
    Vector2D muzzlePos;


    bool automatic = false;
    bool chainsaw = false;
    // Stati: idle, shooting, reloading

    std::string animationIdle = "idle";
    std::string animationReload = "reload";
    std::string animationFire = "shoot";

    std::string soundIdle;
    std::string soundShoot;
    std::string soundReload;
    std::string soundEndfire;


    std::string projectileSprite = "";
    int projectileSize = 4;
    int projectileDamage = 20;
    int projectileGauges = 1;
    int projectileSpreadAngle = 0;
    int projectileSpeed = 1;

    // Tracer trail: attach a TrailComponent to every N-th projectile
    bool tracerEnabled = false;
    int tracerEvery = 3;
    int tracerTickness = 1;
    int tracerLength = 14;

    SDL_Color tracerColor = {124, 126, 123, 255};

    Vector2D attachMargin;


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

    Entity& createWeaponEntity(const std::string& weapon, bool mirrored=false,
                               const std::function<void(WeaponData&)>& tweak={});
private:

    EntityManager& manager;

    Entity& createShotgun(Entity& e);
    Entity& createUzi(Entity& e);
    Entity& createHandgun(Entity &e);
    Entity& createChainsaw(Entity &e);
    Entity& createMachineGun(Entity &e);
};

#endif // WEAPONFACTORY_H
