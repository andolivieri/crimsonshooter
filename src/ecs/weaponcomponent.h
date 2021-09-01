#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H


#include <string>
#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"
#include "projectilecomponent.h"
#include "spritecomponent.h"
#include "relationshipcomponent.h"

class Weapon
{
    std::string texture;
    uint32_t cooldownSecs;
};

class WeaponComponent : public Component
{
public:

    WeaponComponent(const std::string& n);

    WeaponComponent& setAttachPoint(const Vector2D&);

    void init() override;
    void update() override;

    void triggerPull();
    void triggerRelease();

private:
    Vector2D attachPoint;
    bool shooting = false;

    TransformComponent* transform;
    RelationshipComponent* rel;
    SpriteComponent* sprite;
    std::string currentweapon;
    Uint32  lastShot;
    Uint32 cooldown = 1100; //msecs
    int range = 80;

    void equip(const std::string &n);
};

#endif // WEAPONCOMPONENT_H
