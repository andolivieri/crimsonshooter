#include "perkcomponent.h"
#include "../collision.h"
#include "../game.h"

void PerkComponent::init()
{
    transform = &entity->emplaceComponent<TransformComponent>();
    sprite = &entity->emplaceComponent<SpriteComponent>("assets/perk.png");
    collider = &entity->emplaceComponent<ColliderComponent>();
}

void PerkComponent::update()
{

}
