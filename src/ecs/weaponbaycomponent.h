#ifndef WEAPONBAYCOMPONENT_H
#define WEAPONBAYCOMPONENT_H


#include <string>
#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"
#include "projectilecomponent.h"
#include "spritecomponent.h"
#include "relationshipcomponent.h"
#include "inputcomponent.h"
#include "fsmcomponent.h"
#include "soundcomponent.h"
#include "../weaponfactory.h"


class WeaponBayComponent : public Component
{
public:

    WeaponBayComponent(const std::string& n);

    WeaponBayComponent& setAttachPoint(const Vector2D&);

    void init() override;
    void update() override;

    void triggerPull();
    void triggerRelease();

private:
    Vector2D attachPoint;
    bool shooting = false;

    TransformComponent* transform;
    InputComponent* input;
    RelationshipComponent* rel;
    SpriteComponent* sprite;
    std::string currentweapon;
    Uint32  lastShot;

    Uint32 cooldown = 1350; //msecs
    int range = 80;

    void equip(const std::string &n);
};

#endif // WEAPONBAYCOMPONENT_H
