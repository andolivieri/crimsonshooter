#ifndef PERKSPAWNER_H
#define PERKSPAWNER_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "inputcomponent.h"
#include "scorecollector.h"
#include "perkcomponent.h"


class PerkSpawnerComponent : public Component
{
public:

    int perkEveryKills = 10;

    PerkSpawnerComponent(ScoreData& sd):
        scoreData(sd)
    {

    }

    void update() override
    {
        if(scoreData.kills > 0 && scoreData.kills - killsFromLastPerk > perkEveryKills)
        {
            spawnPerk();
            killsFromLastPerk = scoreData.kills;
        }
    }

    void spawnPerk();


private:
    ScoreData& scoreData;
    int killsFromLastPerk;
};
#endif // PERKSPAWNER_H
