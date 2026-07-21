#include "engine/levelmanager.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <helpers/json.hpp>

using json = nlohmann::json;

std::vector<LevelData> LevelManager::s_levels;
bool LevelManager::s_loaded = false;

LevelData LevelManager::loadManifest(const std::string& id, const std::string& path)
{
    LevelData level;
    level.id = id;

    std::ifstream in(path);
    if (!in)
    {
        std::cerr << "LevelManager: could not open manifest " << path << std::endl;
        return level;
    }

    json j;
    in >> j;

    level.name = j.value("name", id);
    level.order = j.value("order", 0);
    level.mapPath = j.value("map", std::string{});

    if (j.contains("waves"))
    {
        for (const auto& w : j["waves"])
        {
            FoeWave wave{
                w.value("kind", std::string{"standard"}),
                w.value("count", 0),
                w.value("maxOnScreen", 1)};
            level.waves.push_back(wave);
        }
    }

    return level;
}

void LevelManager::load(const std::string& indexPath)
{
    if (s_loaded)
    {
        return;
    }
    s_loaded = true;

    std::ifstream in(indexPath);
    if (!in)
    {
        std::cerr << "LevelManager: could not open index " << indexPath << std::endl;
        return;
    }

    json j;
    in >> j;

    if (j.contains("levels"))
    {
        for (const auto& entry : j["levels"])
        {
            const std::string id = entry.value("id", std::string{});
            const std::string manifest = entry.value("manifest", std::string{});
            if (id.empty() || manifest.empty())
            {
                continue;
            }
            s_levels.push_back(loadManifest(id, manifest));
        }
    }

    std::sort(s_levels.begin(), s_levels.end(),
              [](const LevelData& a, const LevelData& b) { return a.order < b.order; });

    std::cout << "LevelManager: loaded " << s_levels.size() << " level(s)" << std::endl;
}

const std::vector<LevelData>& LevelManager::levels()
{
    return s_levels;
}

const LevelData* LevelManager::get(const std::string& id)
{
    for (const auto& lvl : s_levels)
    {
        if (lvl.id == id)
        {
            return &lvl;
        }
    }
    return nullptr;
}

const LevelData* LevelManager::next(const std::string& id)
{
    for (size_t i = 0; i < s_levels.size(); ++i)
    {
        if (s_levels[i].id == id && i + 1 < s_levels.size())
        {
            return &s_levels[i + 1];
        }
    }
    return nullptr;
}
