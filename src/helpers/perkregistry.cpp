#include "helpers/perkregistry.h"

namespace
{
    const std::vector<PerkData>& table()
    {
        static const std::vector<PerkData> perks = [] {
            std::vector<PerkData> m;

            
            m.push_back({"uzi", PerkType::WEAPON, PerkRarity::COMMON, ""});
            m.push_back({"shotgun", PerkType::WEAPON, PerkRarity::COMMON, ""});
            m.push_back({"mg", PerkType::WEAPON, PerkRarity::RARE, ""});

            m.push_back({"health", PerkType::OTHER, PerkRarity::COMMON, "assets/life.png"});
            m.push_back({"grenade", PerkType::OTHER, PerkRarity::COMMON, "assets/grenade.png"});
            m.push_back({"fire", PerkType::OTHER, PerkRarity::RARE, "assets/fire.png"});
            m.push_back({"strafe", PerkType::OTHER, PerkRarity::COMMON, "assets/skull.png"});
            m.push_back({"nuke", PerkType::OTHER, PerkRarity::EPIC, "assets/nuke.png"});
            m.push_back({"turret", PerkType::OTHER, PerkRarity::RARE, "assets/turret.png"});

            return m;
        }();
        return perks;
    }
}

const std::vector<PerkData>& PerkRegistry::all()
{
    return table();
}

const PerkData& PerkRegistry::get(const std::string& name)
{
    for (const auto& perk : table())
    {
        if (perk.name == name)
        {
            return perk;
        }
    }
    // fallback: unknown perks are common "other" perks with no icon
    static const PerkData unknown{};
    return unknown;
}

const PerkRarityRates& PerkRegistry::defaultRates()
{
    static const PerkRarityRates rates{};
    return rates;
}

bool PerkRegistry::rarityFromString(const std::string& name, PerkRarity& out)
{
    if (name == "common")
    {
        out = PerkRarity::COMMON;
        return true;
    }
    if (name == "rare")
    {
        out = PerkRarity::RARE;
        return true;
    }
    if (name == "epic")
    {
        out = PerkRarity::EPIC;
        return true;
    }
    return false;
}

PerkRarity PerkRegistry::pickRarity(const bool nonEmpty[PERK_RARITY_COUNT],
                                    const PerkRarityRates& rates,
                                    float roll)
{
    // commonest bucket that has perks in it: it takes whatever is left over
    int fallback = -1;
    for (int r = 0; r < PERK_RARITY_COUNT; ++r)
    {
        if (nonEmpty[r])
        {
            fallback = r;
            break;
        }
    }
    if (fallback < 0)
    {
        return PerkRarity::COMMON;
    }

    // walk rarest -> commonest, consuming the roll with each bucket's own chance
    float acc = 0.0f;
    for (int r = PERK_RARITY_COUNT - 1; r > fallback; --r)
    {
        if (!nonEmpty[r])
        {
            continue;
        }
        acc += rates.chance(static_cast<PerkRarity>(r));
        if (roll < acc)
        {
            return static_cast<PerkRarity>(r);
        }
    }
    return static_cast<PerkRarity>(fallback);
}
