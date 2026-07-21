#include "helpers/foeregistry.h"

#include <unordered_map>

namespace
{
    // meyers singleton
    const std::unordered_map<std::string, FoeKindData>& table()
    {
        static const std::unordered_map<std::string, FoeKindData> kinds = [] {
            std::unordered_map<std::string, FoeKindData> m;

            FoeKindData standard;
            m["standard"] = standard;

            FoeKindData brute;
            brute.health = 90;
            brute.speedMin = 0.2f;
            brute.speedMax = 0.6f;
            brute.contactDamage = 20;
            m["brute"] = brute;

            FoeKindData runner;
            runner.health = 12;
            runner.speedMin = 1.4f;
            runner.speedMax = 2.4f;
            runner.contactDamage = 6;
            m["runner"] = runner;

            return m;
        }();
        return kinds;
    }
}

const FoeKindData& FoeRegistry::get(const std::string& kind)
{
    const auto& kinds = table();
    auto it = kinds.find(kind);
    if (it != kinds.end())
    {
        return it->second;
    }
    // fallback
    return kinds.at("standard");
}
