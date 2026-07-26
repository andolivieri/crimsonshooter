#include "engine/levelmanager.h"

#include "engine/resources.h"

#include <algorithm>
#include <iostream>
#include <helpers/json.hpp>

using json = nlohmann::json;

std::vector<LevelData> LevelManager::s_levels;
bool LevelManager::s_loaded = false;

LevelData LevelManager::loadManifest(const std::string& id, const std::string& path)
{
    LevelData level;
    level.id = id;

    if (!Resources::find(path))
    {
        std::cerr << "LevelManager: could not open manifest " << path << std::endl;
        return level;
    }

    json j = json::parse(Resources::readString(path));

    level.name = j.value("name", id);
    level.order = j.value("order", 0);
    level.mapPath = j.value("map", std::string{});

    // list of perk names to draw the starting perk from
    if (j.contains("startingPerks"))
    {
        for (const auto& perk : j["startingPerks"])
        {
            level.startingPerks.push_back(perk.get<std::string>());
        }
    }

    // optional per-level perk tuning, on top of the PerkRegistry defaults:
    //   "perks": {
    //       "rarity":  { "common": 10, "rare": 50, "epic": 100 },   // one in N
    //       "buckets": { "epic": ["nuke", "turret"] },              // re-bucket perks
    //       "pool":    ["uzi", "health", "nuke"]                    // restrict what drops
    //   }
    level.perks.rates = PerkRegistry::defaultRates();
    if (j.contains("perks"))
    {
        const auto& p = j["perks"];

        if (p.contains("rarity"))
        {
            for (auto it = p["rarity"].begin(); it != p["rarity"].end(); ++it)
            {
                PerkRarity rarity;
                if (PerkRegistry::rarityFromString(it.key(), rarity))
                {
                    level.perks.rates.setOneIn(rarity, it.value().get<float>());
                }
                else
                {
                    std::cerr << "LevelManager: unknown perk rarity '" << it.key() << "' in " << path
                              << std::endl;
                }
            }
        }

        if (p.contains("buckets"))
        {
            for (auto it = p["buckets"].begin(); it != p["buckets"].end(); ++it)
            {
                PerkRarity rarity;
                if (!PerkRegistry::rarityFromString(it.key(), rarity))
                {
                    std::cerr << "LevelManager: unknown perk rarity '" << it.key() << "' in " << path
                              << std::endl;
                    continue;
                }
                for (const auto& perk : it.value())
                {
                    level.perks.rarityOverrides.emplace_back(perk.get<std::string>(), rarity);
                }
            }
        }

        if (p.contains("pool"))
        {
            for (const auto& perk : p["pool"])
            {
                level.perks.pool.push_back(perk.get<std::string>());
            }
        }
    }

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

    if (!Resources::find(indexPath))
    {
        std::cerr << "LevelManager: could not open index " << indexPath << std::endl;
        return;
    }

    json j = json::parse(Resources::readString(indexPath));

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
