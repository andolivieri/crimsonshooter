#ifndef STATCOLLECTOR_H
#define STATCOLLECTOR_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "damagemodel.h"


struct ScoreData
{
    int kills = 0;
    int score = 0;
};

class ScoreCollector : public Component
{
public:

    ScoreCollector(ScoreData& s):
        scoreData(s)
    {

    }

    void init() override
    {
        damage = &entity->emplaceComponent<DamageModelComponent>();
    }

    void update() override
    {
        if(damage->diedNow())
        {
            scoreData.kills++;
        }
    }



private:
    ScoreData& scoreData;
    DamageModelComponent* damage;
    int dummyValue;
    time_t startTime;

};

#endif // STATCOLLECTOR_H
