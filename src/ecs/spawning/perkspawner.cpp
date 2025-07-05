#include "perkspawner.h"
#include "engine/game.h"
#include "helpers/utils.h"
#include "helpers/collision.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cctype>

void PerkSpawnerComponent::init()
{
    killsFromLastPerk = 0;
    hasPickedFirstWeaponPerk = false;
    totalPerksPickedUp = 0;
    perkHistory.clear();
}

void PerkSpawnerComponent::update()
{


    if(!startPerkSpawned){
        createPerkEntity("bomber", PerkType::OTHER, {200,200});
        startPerkSpawned = true;
    }

    if(scoreData.kills > 0 && scoreData.kills - killsFromLastPerk > perkEveryKills)
    {
        spawnPerk();
        killsFromLastPerk = scoreData.kills;
    }
}

std::string PerkSpawnerComponent::drawPerk()
{

    const bool mustDrawWeapon = 
        !hasPickedFirstWeaponPerk || (totalPerksPickedUp > 0 && totalPerksSpawned % 5 == 0);
    if (mustDrawWeapon) {
        return weaponPerks[rand() % weaponPerks.size()];
    }
    
    
    return otherPerks[rand() % otherPerks.size()];
}

void PerkSpawnerComponent::createPerkEntity(const std::string& perkName, PerkType perkType, const Vector2D& position)
{
    auto& perk = entity->m_manager.addEntity();
    const auto decayTime = 10*ONE_SECOND;
    const auto width = 32;
    const auto height = 32;

    perk.addComponent<TransformComponent>(position.x, position.y, width, height);
    perk.addComponent<DecayComponent>(decayTime);
    perk.addComponent<PerkComponent>(perkName, perkType);
    perk.emplaceComponent<SpriteComponent>("assets/perk.png").setSrcRect({0,0, 64,64});
    
    auto& perkSpawner = *this;
    perk.emplaceComponent<ColliderComponent>().onCollision([&perkSpawner, &perk, perkName, perkType](Entity& target){
        Entity* player = target.m_manager.get("player");
        if(&target == player){
            
            auto& perkComponent = perk.getComponent<PerkComponent>();
            perkComponent.applyPerk(*player);
            
           
            perkSpawner.totalPerksPickedUp++;
            perkSpawner.perkHistory.push_back(perkName);
            
            if (perkType == PerkType::WEAPON) {
                perkSpawner.hasPickedFirstWeaponPerk = true;
            }

            perk.setActive(false);
        }
    });

    if (perkType == PerkType::WEAPON) {
        // Display first letter of perk name on sprite for weapon perks
        std::string displayLetter = perkName.substr(0, 1);
        std::transform(displayLetter.begin(), displayLetter.end(), displayLetter.begin(), ::toupper);
        perk.addComponent<TextComponent>(displayLetter, SDL_Rect({8, 8, 16, 16})).setAbsolute(false);
    } else {
        // For OTHER perks, use icons
        perk.addComponent<RelationshipComponent>();
        
        auto& icon = entity->m_manager.addEntity();
        icon.addComponent<TransformComponent>(position.x, position.y, width, height);
        
        std::string iconSprite;
        if (perkName == "health") {
            iconSprite = "assets/life.png";
        } else if (perkName == "grenade") {
            iconSprite = "assets/grenade.png";
        } else if (perkName == "fire") {
            iconSprite = "assets/fire.png";
        } else if (perkName == "bomber") {
            iconSprite = "assets/nuke.png";
        }
        
        if (!iconSprite.empty()) {
            icon.emplaceComponent<SpriteComponent>(iconSprite)
                .setTransform(&perk.getComponent<TransformComponent>());
            icon.addComponent<DecayComponent>(decayTime);
            icon.addGroup(groupPerksIcon);
            
            perk.getComponent<RelationshipComponent>().addChildren(&icon, "icon");
        }
    }

    perk.addGroup(groupPerks);
}

void PerkSpawnerComponent::spawnPerk()
{
    std::string selectedPerk = drawPerk();
    Vector2D spawnPt = scoreData.lastKillPosition;
    
    PerkType perkType = PerkType::OTHER;
    for (const auto& weapon : weaponPerks) {
        if (weapon == selectedPerk) {
            perkType = PerkType::WEAPON;
            break;
        }
    }
    
    std::cout << "Spawning perk: " << selectedPerk << " (type: " 
              << (perkType == PerkType::WEAPON ? "WEAPON" : "OTHER") << ")" << std::endl;
    
    createPerkEntity(selectedPerk, perkType, spawnPt);
    totalPerksSpawned++;
}
