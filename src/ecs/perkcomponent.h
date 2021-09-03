#ifndef PERKCOMPONENT_H
#define PERKCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "inputcomponent.h"
#include "collidercomponent.h"

class PerkComponent : public Component
{
public:

    std::string weapon;
    PerkComponent(const std::string w): weapon(w){}

    void init() override;
    void update() override;


private:
    TransformComponent* transform;
    SpriteComponent* sprite;
    ColliderComponent* collider;

};
#endif // PERKCOMPONENT_H
