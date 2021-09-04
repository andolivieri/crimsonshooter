#include "perkspawner.h"
#include "../game.h"

static std::array<std::string, 3> weaponz{"pistol", "uzi", "shotgun"};

void PerkSpawnerComponent::spawnPerk()
{
    auto& theFoe = entity->m_manager.addEntity();

    auto width = 32;
    auto height = 32;

    std::string w = weaponz[rand() % weaponz.size()];

    Vector2D spawnPt;
    spawnPt.x = static_cast<float>(rand() % Game::winWidth);
    spawnPt.y = static_cast<float>(rand() % Game::winHeigth);

    theFoe.addComponent<TransformComponent>(spawnPt.x,spawnPt.y, width, height);
    theFoe.addComponent<PerkComponent>(w);
    theFoe.addComponent<DecayComponent>(10);
    theFoe.getComponent<SpriteComponent>().setSrcRect({0,0, 64,64});
    theFoe.addComponent<TextComponent>(w.substr(0, 1), SDL_Rect({8, 8, 16, 16}));

    theFoe.addGroup(groupPerks);



}
