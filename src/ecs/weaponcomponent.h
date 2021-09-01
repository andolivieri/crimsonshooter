#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H


#include <string>
#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"
#include "projectilecomponent.h"
#include "spritecomponent.h"

class Weapon
{
    std::string texture;
    uint32_t cooldownSecs;
};

class WeaponComponent : public Component
{
public:

    WeaponComponent(const std::string n);

    void init() override;
    void update() override;

    void triggerPull();
    void triggerRelease();

private:
    bool shooting = false;

    TransformComponent* transform;
    SpriteComponent* sprite;
    std::string name;
    Uint32  lastShot;
    Uint32 cooldown = 1000; //msecs
    int range = 80;

};

#endif // WEAPONCOMPONENT_H
