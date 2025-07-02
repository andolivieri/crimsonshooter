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
    int startPerkSpawned = false;
    
    bool hasPickedFirstWeaponPerk = false;
    int totalPerksPickedUp = 0;
    int totalPerksSpawned = 0;
    std::vector<std::string> perkHistory;
    
    std::vector<std::string> weaponPerks = {
        //"pistol", 
        // "chainsaw"
        "uzi", "shotgun"};
    std::vector<std::string> otherPerks = {"fire", "grenade", "health"};
    
    std::string drawPerk();
    void createPerkEntity(const std::string& perkName, PerkType perkType, const Vector2D& position);
};
#endif // PERKSPAWNER_H
