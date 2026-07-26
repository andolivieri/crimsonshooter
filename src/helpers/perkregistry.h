#ifndef PERKREGISTRY_H
#define PERKREGISTRY_H

#include <string>
#include <utility>
#include <vector>

enum class PerkType
{
    WEAPON,
    OTHER
};

// Rarity buckets, ascending: COMMON is the most likely one
enum class PerkRarity
{
    COMMON = 0,
    RARE,
    EPIC,
    COUNT
};

constexpr int PERK_RARITY_COUNT = static_cast<int>(PerkRarity::COUNT);

// Per-bucket draw chances, expressed as "one in N" (N = 100 -> 1% of draws).
// The rates are absolute: a bucket's chance does not depend on how many perks
// live in it. Whatever probability is left over goes to the commonest bucket
// that actually has perks available, so the configured value for COMMON is a
// floor rather than an exact rate.
struct PerkRarityRates
{
    float oneIn[PERK_RARITY_COUNT] = {10.0f, 50.0f, 100.0f};

    float chance(PerkRarity rarity) const
    {
        const float n = oneIn[static_cast<int>(rarity)];
        return n > 0.0f ? 1.0f / n : 0.0f;
    }

    // skills/upgrades can override the distribution
    void setOneIn(PerkRarity rarity, float n) { oneIn[static_cast<int>(rarity)] = n; }
};

struct PerkData
{
    std::string name;
    PerkType type = PerkType::OTHER;
    PerkRarity rarity = PerkRarity::COMMON;
    std::string icon; // empty -> perk renders its initial instead of an icon
};

// Per-level tweaks
struct PerkPoolConfig
{
    PerkRarityRates rates;
    std::vector<std::pair<std::string, PerkRarity>> rarityOverrides;
    std::vector<std::string> pool; // empty -> every registered perk is drawable
};

class PerkRegistry
{
public:
    static const PerkData& get(const std::string& name);
    static const std::vector<PerkData>& all();
    static const PerkRarityRates& defaultRates();

    static bool rarityFromString(const std::string& name, PerkRarity& out);

    // Picks a bucket from a [0,1) roll
    static PerkRarity pickRarity(const bool nonEmpty[PERK_RARITY_COUNT],
                                 const PerkRarityRates& rates,
                                 float roll);
};

#endif // PERKREGISTRY_H
