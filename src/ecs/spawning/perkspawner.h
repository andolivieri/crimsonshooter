#ifndef PERKSPAWNER_H
#define PERKSPAWNER_H

#include <time.h>
#include <SDL.h>
#include <vector>
#include <string>
#include "ecs/ecs.h"
#include "ecs/input/inputcomponent.h"
#include "ecs/gameplay/scorecollector.h"
#include "ecs/base/relationshipcomponent.h"
#include "helpers/perkregistry.h"
#include "perkcomponent.h"


class PerkSpawnerComponent : public Component
{
public:

    int perkEveryKills = 10;

    PerkSpawnerComponent(ScoreData& sd,
                         std::vector<std::string> startingPerkPool = {},
                         PerkPoolConfig perkConfig = {}):
        scoreData(sd),
        startingPerkPool(std::move(startingPerkPool)),
        perkConfig(std::move(perkConfig))
    {

    }

    void init() override;
    void update() override;

    void spawnPerk();

    PerkRarityRates& rates() { return perkConfig.rates; }

private:
    ScoreData& scoreData;
    int killsFromLastPerk;
    int startPerkSpawned = false;
    
    bool hasPickedFirstWeaponPerk = false;
    int totalPerksPickedUp = 0;
    int totalPerksSpawned = 0;
    std::vector<std::string> perkHistory;

    // drawable perks, built in init() from PerkRegistry + the level's pool filter
    std::vector<std::string> weaponPerks;
    std::vector<std::string> otherPerks;

    // starting perks pool, emtpu = "any weapon".
    std::vector<std::string> startingPerkPool;

    PerkPoolConfig perkConfig;

    void buildPools();
    PerkRarity rarityFor(const std::string& perkName) const;
    std::string drawFromPool(const std::vector<std::string>& pool) const;
    std::string drawPerk();
    std::string drawStartingPerk();
    PerkType perkTypeFor(const std::string& perkName) const;
    void createPerkEntity(const std::string& perkName, PerkType perkType, const Vector2D& position);
};
#endif // PERKSPAWNER_H
