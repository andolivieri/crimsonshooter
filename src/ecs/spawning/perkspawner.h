#ifndef PERKSPAWNER_H
#define PERKSPAWNER_H

#include <time.h>
#include <SDL.h>
#include <vector>
#include <string>
#include "ecs.h"
#include "ecs/input/inputcomponent.h"
#include "ecs/gameplay/scorecollector.h"
#include "perkcomponent.h"


class PerkSpawnerComponent : public Component
{
public:

    int perkEveryKills = 10;

    PerkSpawnerComponent(ScoreData& sd):
        scoreData(sd)
    {

    }

    void init() override;
    void update() override;

    void spawnPerk();

private:
    ScoreData& scoreData;
    int killsFromLastPerk;
    
    // Perk tracking
    bool hasPickedFirstWeaponPerk = false;
    int totalPerksPickedUp = 0;
    int totalPerksSpawned = 0;
    std::vector<std::string> perkHistory;
    
    // Perk pools
    std::vector<std::string> weaponPerks = {"pistol", "uzi", "shotgun", "chainsaw"};
    std::vector<std::string> otherPerks = {"health", "grenade"};
    
    std::string drawPerk();
    void createPerkEntity(const std::string& perkName, PerkType perkType, const Vector2D& position);
};
#endif // PERKSPAWNER_H
