#ifndef PERKCOMPONENT_H
#define PERKCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs/ecs.h"
#include "ecs/input/inputcomponent.h"
#include "ecs/base/collidercomponent.h"
#include "helpers/perkregistry.h" // PerkType

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
