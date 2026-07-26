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
    buildPools();
}

void PerkSpawnerComponent::buildPools()
{
    weaponPerks.clear();
    otherPerks.clear();

    for (const auto& perk : PerkRegistry::all())
    {
        // level might restrict pool
        if (!perkConfig.pool.empty())
        {
            const auto& pool = perkConfig.pool;
            if (std::find(pool.begin(), pool.end(), perk.name) == pool.end())
            {
                continue;
            }
        }

        if (perk.type == PerkType::WEAPON)
        {
            weaponPerks.push_back(perk.name);
        }
        else
        {
            otherPerks.push_back(perk.name);
        }
    }
}

void PerkSpawnerComponent::update()
{


    if(!startPerkSpawned){
        const std::string startPerk = drawStartingPerk();
        createPerkEntity(startPerk, perkTypeFor(startPerk), {Game::winWidth/2,Game::winHeigth/2 - 150});
        startPerkSpawned = true;
    }

    if(scoreData.kills > 0 && scoreData.kills - killsFromLastPerk > perkEveryKills)
    {
        spawnPerk();
        killsFromLastPerk = scoreData.kills;
    }
}

PerkRarity PerkSpawnerComponent::rarityFor(const std::string& perkName) const
{
    // level overrides win over the registry defaults
    for (const auto& override : perkConfig.rarityOverrides)
    {
        if (override.first == perkName)
        {
            return override.second;
        }
    }
    return PerkRegistry::get(perkName).rarity;
}

std::string PerkSpawnerComponent::drawFromPool(const std::vector<std::string>& pool) const
{
    if (pool.empty())
    {
        return {};
    }

    // split the pool per bucket, then roll for a bucket and pick uniformly inside it
    std::vector<std::string> buckets[PERK_RARITY_COUNT];
    for (const auto& perkName : pool)
    {
        buckets[static_cast<int>(rarityFor(perkName))].push_back(perkName);
    }

    bool nonEmpty[PERK_RARITY_COUNT];
    for (int r = 0; r < PERK_RARITY_COUNT; ++r)
    {
        nonEmpty[r] = !buckets[r].empty();
    }

    const float roll = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) + 1.0f);
    const auto& picked = buckets[static_cast<int>(PerkRegistry::pickRarity(nonEmpty, perkConfig.rates, roll))];

    return picked[rand() % picked.size()];
}

std::string PerkSpawnerComponent::drawPerk()
{

    const bool mustDrawWeapon =
        !hasPickedFirstWeaponPerk || (totalPerksPickedUp > 0 && totalPerksSpawned % 5 == 0);
    if (mustDrawWeapon && !weaponPerks.empty()) {
        return drawFromPool(weaponPerks);
    }


    return drawFromPool(otherPerks.empty() ? weaponPerks : otherPerks);
}

std::string PerkSpawnerComponent::drawStartingPerk()
{
    // Draw from the level-provided pool; fall back to weapons if none given.
    return drawFromPool(startingPerkPool.empty() ? weaponPerks : startingPerkPool);
}

PerkType PerkSpawnerComponent::perkTypeFor(const std::string& perkName) const
{
    return PerkRegistry::get(perkName).type;
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
        
        const std::string& iconSprite = PerkRegistry::get(perkName).icon;

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
    
    PerkType perkType = perkTypeFor(selectedPerk);

    createPerkEntity(selectedPerk, perkType, spawnPt);
    totalPerksSpawned++;
}
