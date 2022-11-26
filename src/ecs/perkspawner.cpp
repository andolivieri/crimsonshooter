#include "perkspawner.h"
#include "../game.h"
#include "../utils.h"

static std::array<std::string, 3> weaponz{"pistol", "uzi", "shotgun"};

void PerkSpawnerComponent::init()
{
    killsFromLastPerk = 0;
}

void PerkSpawnerComponent::update()
{

    if(scoreData.kills > 0 && scoreData.kills - killsFromLastPerk > perkEveryKills)
    {
        spawnPerk();
        killsFromLastPerk = scoreData.kills;
    }
}

void PerkSpawnerComponent::spawnPerk()
{
    auto& theFoe = entity->m_manager.addEntity();

    auto width = 32;
    auto height = 32;

    std::string w = weaponz[rand() % weaponz.size()];

    Vector2D spawnPt = scoreData.lastKillPosition;

    theFoe.addComponent<TransformComponent>(spawnPt.x,spawnPt.y, width, height);
    theFoe.addComponent<PerkComponent>(w);
    theFoe.addComponent<DecayComponent>(10*TIME_SECOND);
    theFoe.getComponent<SpriteComponent>().setSrcRect({0,0, 64,64});
    theFoe.addComponent<TextComponent>(w.substr(0, 1), SDL_Rect({8, 8, 16, 16}));

    theFoe.addGroup(groupPerks);

    std::cout << "Spawning at " << spawnPt << std::endl;


}
