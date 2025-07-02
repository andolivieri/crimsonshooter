#ifndef FOESPAWNER_H
#define FOESPAWNER_H


#include <time.h>
#include <SDL.h>
#include "ecs/ecs.h"
#include "ecs/gameplay/scorecollector.h"


struct FoeWave
{
    const std::string whatkind;
    int total;
    int maxOnScreen;

};

class FoeSpawnerComponent : public Component
{
public:

    FoeSpawnerComponent(ScoreData& sd):
        scoreData(sd)
    {

    }

    FoeSpawnerComponent& addWave(const FoeWave& w)
    {
        totalEnemies += w.total;
        waves.push_back(w);
        return *this;
    }

    void update() override;

    bool allEnemiesDefeated() const;

    bool isFinished()
    {
        return finished;
    }

private:
    bool finished = false;

    ScoreData& scoreData;
    std::deque<FoeWave> waves;
    int currentWaveSpawnCount = 0;
    int currentWaveSpawnKills = 0;
    int totalEnemies = 0;
    bool done = false;

    void spawnFoe(const std::string foekind);
};
#endif // FOESPAWNER_H
