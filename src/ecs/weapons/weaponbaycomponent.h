#ifndef WEAPONBAYCOMPONENT_H
#define WEAPONBAYCOMPONENT_H


#include <string>
#include <time.h>
#include <SDL.h>
#include "ecs/ecs.h"
#include "ecs/base/transformcomponent.h"
#include "projectilecomponent.h"
#include "ecs/base/spritecomponent.h"
#include "ecs/base/relationshipcomponent.h"
#include "ecs/input/inputcomponent.h"
#include "ecs/base/fsmcomponent.h"
#include "ecs/effects/soundcomponent.h"
#include "helpers/weaponfactory.h"


class AttachedWeapon
{
public:
    std::string weaponId;
    Entity* entity = nullptr;
    Vector2D attachPoint;
};

class WeaponBayComponent : public Component
{
public:

    WeaponBayComponent();

    WeaponBayComponent& setAttachPoint(const Vector2D&, int slot=0, bool mirrored=false);
    WeaponBayComponent& equip(const std::string &n, int slot=0);
    WeaponBayComponent& autoequip(const std::string& n);

    void init() override;
    void update() override;

    void drop(int slot);

    int grenadeCount;

private:

    std::array<PlayerControl, 2> bindKeys;
    std::array<bool, 2> mirror;
    std::array<Vector2D, 2> slots;
    std::array<AttachedWeapon, 2> weapons;

    TransformComponent* transform;
    InputComponent* input;
    RelationshipComponent* rel;
    SpriteComponent* sprite;

};

#endif // WEAPONBAYCOMPONENT_H
