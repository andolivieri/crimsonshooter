#include "perkcomponent.h"
#include "collision.h"
#include "game.h"
#include "ecs/weapons/weaponbaycomponent.h"
#include "ecs/gameplay/damagemodel.h"
#include <algorithm>
#include <iostream>

void PerkComponent::init()
{
    transform = &entity->emplaceComponent<TransformComponent>();
    sprite = &entity->emplaceComponent<SpriteComponent>("assets/perk.png");
    collider = &entity->emplaceComponent<ColliderComponent>();
}

void PerkComponent::update()
{

}

void PerkComponent::applyPerk(Entity& player)
{
    switch (perkType) {
        case PerkType::WEAPON:
            // Apply weapon perk
            if (player.hasComponent<WeaponBayComponent>()) {
                player.getComponent<WeaponBayComponent>().autoequip(perkName);
                std::cout << "Applied weapon perk: " << perkName << std::endl;
            }
            break;
            
        case PerkType::OTHER:
            // Apply other perks
            if (perkName == "health") {
                if (player.hasComponent<DamageModelComponent>()) {
                    auto& damageModel = player.getComponent<DamageModelComponent>();
                    int halfMaxHealth = damageModel.startHealth / 2;
                    int newHealth = damageModel.health + halfMaxHealth;
                    // Don't exceed initial health
                    damageModel.health = std::min(newHealth, damageModel.startHealth);
                    std::cout << "Applied health perk: restored " << halfMaxHealth 
                              << " health (current: " << damageModel.health << ")" << std::endl;
                }
            } else if (perkName == "grenade") {
                if (player.hasComponent<WeaponBayComponent>()) {
                    auto& weaponBay = player.getComponent<WeaponBayComponent>();
                    weaponBay.grenadeCount += 3; // Add 3 grenades
                    std::cout << "Applied grenade perk: +3 grenades (total: " 
                              << weaponBay.grenadeCount << ")" << std::endl;
                }
            }
            break;
    }
}
