#include "perkspawner.h"
#include "game.h"
#include "utils.h"
#include "collision.h"
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

    auto width = 32;
    auto height = 32;

    perk.addComponent<TransformComponent>(position.x, position.y, width, height);
    perk.addComponent<DecayComponent>(10*TIME_SECOND);
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

    // Display first letter of perk name on sprite
    std::string displayLetter = perkName.substr(0, 1);
    std::transform(displayLetter.begin(), displayLetter.end(), displayLetter.begin(), ::toupper);
    perk.addComponent<TextComponent>(displayLetter, SDL_Rect({8, 8, 16, 16})).setAbsolute(false);

    perk.addGroup(groupPerks);
}

void PerkSpawnerComponent::spawnPerk()
{
    std::string selectedPerk = drawPerk();
    Vector2D spawnPt = scoreData.lastKillPosition;
    
    // Determine perk type
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
