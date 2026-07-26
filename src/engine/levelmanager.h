#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <string>
#include <vector>
#include "ecs/spawning/foespawnercomponent.h" // FoeWave
#include "helpers/perkregistry.h"             // PerkPoolConfig

struct LevelData
{
    std::string id;
    std::string name;
    int order = 0;
    std::string mapPath;
    std::vector<FoeWave> waves;
    std::vector<std::string> startingPerks;
    PerkPoolConfig perks;
};

class LevelManager
{
public:
    static void load(const std::string& indexPath = "assets/levels/index.json");

    static const std::vector<LevelData>& levels();
    static const LevelData* get(const std::string& id);
    static const LevelData* next(const std::string& id);

private:
    static std::vector<LevelData> s_levels;
    static bool s_loaded;

    static LevelData loadManifest(const std::string& id, const std::string& path);
};

#endif // LEVELMANAGER_H
