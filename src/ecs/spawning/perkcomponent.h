#ifndef PERKCOMPONENT_H
#define PERKCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "ecs/input/inputcomponent.h"
#include "ecs/core/collidercomponent.h"

enum class PerkType {
    WEAPON,
    OTHER
};

class PerkComponent : public Component
{
public:

    std::string perkName;
    PerkType perkType;
    
    PerkComponent(const std::string& name, PerkType type): 
        perkName(name), perkType(type) {}

    void init() override;
    void update() override;
    void applyPerk(Entity& player);

private:
    TransformComponent* transform;
    SpriteComponent* sprite;
    ColliderComponent* collider;

};
#endif // PERKCOMPONENT_H
