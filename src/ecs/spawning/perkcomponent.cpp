#include "perkcomponent.h"
#include "helpers/collision.h"
#include "engine/game.h"
#include "ecs/weapons/weaponbaycomponent.h"
#include "ecs/gameplay/damagemodel.h"
#include "ecs/effects/firecomponent.h"
#include "ecs/effects/bombruncomponent.h"
#include "ecs/effects/straferuncomponent.h"
#include "ecs/effects/turretcomponent.h"
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
            } else if (perkName == "fire") {
                /*
                Vector2D ringCenter = player.getComponent<TransformComponent>().center();
                auto &ring = player.m_manager.addEntity();
                ring.addComponent<TransformComponent>(0,0, 128, 128).centerOn(ringCenter);
                ring.addComponent<SpriteComponent>("assets/ring_of_fire.png")
                    .setSrcRect({0,0,128,128})
                    .addAnimation("burn", {0,0,3,50})
                    .play("burn");
                ring.addComponent<DecayComponent>(3 * ONE_SECOND);
                

                ring.addComponent<DumbComponent>().onUpdate([ringCenter, &ring](){
                    auto& bgTransform = ring.getComponent<TransformComponent>();
                    bgTransform.width    *= 1.03;
                    bgTransform.height *= 1.03;
                    bgTransform.centerOn(ringCenter);
                });

                ring.addGroup(groupFlames);
                */



                auto enemies = player.m_manager.getGroup(groupEnemies);
                int enemiesSet = 0;
                for (auto enemy : enemies) {
                    if (!enemy->hasComponent<FireComponent>() && enemy->hasComponent<DamageModelComponent>()) {
                        if(!enemy->getComponent<DamageModelComponent>().isDead()){
                            enemy->addComponent<FireComponent>(10.0f * ONE_SECOND, 5);
                            enemiesSet++;
                        }
                    }
                }
            } else if (perkName == "nuke") {
                // Create a bomb run entity that will spawn the bomber after a delay
                auto& bombRunEntity = player.m_manager.addEntity("bomb_run_controller");
                bombRunEntity.addComponent<BombRunComponent>(transform->pos,  1.0f); // 1 second delay
                std::cout << "Applied bomber perk: bomber incoming!" << std::endl;
            } else if (perkName == "strafe") {
                // A10 strafe run: brrrrt, advancing rain of bullets, then the shadow passes over.
                auto& strafeEntity = player.m_manager.addEntity("strafe_run_controller");
                StrafeRunComponent::Config cfg;
                cfg.fireDuration = 2.0;
                strafeEntity.addComponent<StrafeRunComponent>
                    (player.getComponent<TransformComponent>().center(), cfg);
                std::cout << "Applied strafe perk: A10 inbound, brrrrt!" << std::endl;
            } else if (perkName == "turret") {
                // Deploy a temporary auto-firing turret at the pickup spot.
                auto& turret = player.m_manager.addEntity("turret");
                turret.addComponent<TransformComponent>(transform->pos.x, transform->pos.y, 64, 64);
                TurretComponent::Config cfg;
                cfg.weaponId = "mg";
                cfg.duration = 3600.0f;
                cfg.overrideMagazine = 1000000;
                turret.addComponent<TurretComponent>(cfg);
                std::cout << "Applied turret perk: turret deployed!" << std::endl;
            }
            break;
    }
}
