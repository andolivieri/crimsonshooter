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
            standard.health = 10;
            standard.colorModifier = {60, 220, 90, 255};
            m["standard"] = standard;

            FoeKindData brute;
            brute.health = 150;
            brute.speedMin = 0.2f;
            brute.speedMax = 0.6f;
            brute.contactDamage = 20;
            brute.colorModifier = {230, 220, 40, 255};
            m["brute"] = brute;

            FoeKindData runner;
            runner.health = 30;
            runner.speedMin = 1.4f;
            runner.speedMax = 2.4f;
            runner.contactDamage = 6;
            runner.colorModifier = {255, 150, 40, 255};
            m["runner"] = runner;

            FoeKindData fast_runner;
            fast_runner.health = 30;
            fast_runner.speedMin = 2.4f;
            fast_runner.speedMax = 5.4f;
            fast_runner.contactDamage = 6;
            fast_runner.colorModifier = {230, 30, 30, 255};
            m["fast_runner"] = fast_runner;


            FoeKindData boss;
            boss.health = 2500;
            boss.speedMin = 1.4f;
            boss.speedMax = 2.4f;
            boss.contactDamage = 12;
            boss.colorModifier = {230, 30, 30, 255};
            m["boss"] = boss;

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
