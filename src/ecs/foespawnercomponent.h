#ifndef FOESPAWNER_H
#define FOESPAWNER_H


#include <time.h>
#include <SDL.h>
#include "ecs.h"


struct FoeWave
{
    const std::string whatkind;
    int total;

};

class FoeSpawnerComponent : public Component
{
public:

    FoeSpawnerComponent(int _maxOnScreen):
        maxOnScreen(_maxOnScreen)
    {

    }

    FoeSpawnerComponent& addWave(const FoeWave& w)
    {
        waves.push_back(w);
        return *this;
    }

    void update() override;


private:
    std::deque<FoeWave> waves;
    int currentWaveSpawnCount = 0;
    bool done = false;
    int maxOnScreen;

    void spawnFoe(const std::string foekind);
};
#endif // FOESPAWNER_H
