#include "perkspawner.h"
#include "../game.h"


void PerkSpawnerComponent::spawnPerk()
{
    auto& theFoe = entity->m_manager.addEntity();



    Vector2D spawnPt;
    spawnPt.x = static_cast<float>(rand() % Game::winWidth);
    spawnPt.y = static_cast<float>(rand() % Game::winHeigth);

    theFoe.addComponent<TransformComponent>(spawnPt.x,spawnPt.y, 32,32);
    theFoe.addComponent<PerkComponent>("shotgun");
    theFoe.getComponent<SpriteComponent>().setSrcRect({0,0, 64,64});

    theFoe.addGroup(groupPerks);



}
