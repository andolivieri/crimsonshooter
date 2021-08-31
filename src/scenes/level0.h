#ifndef LEVEL1_H
#define LEVEL1_H

#include "../game.h"
#include "../gamemap.h"
#include "../ecs.h"
#include "../ecs/components.h"

void stuff(EntityManager& manager)
{


    GameMap* g_map = nullptr;

    auto& newPlayer(manager.addEntity());

    g_map = new GameMap();

    GameMap::LoadMap("assets/themap.json");

    newPlayer.addComponent<TransformComponent>(100.f,100.f, 64,64);
    newPlayer.getComponent<TransformComponent>().speed = 2;
    newPlayer.getComponent<TransformComponent>().width = 64;
    newPlayer.getComponent<TransformComponent>().height = 64;
    newPlayer.getComponent<TransformComponent>().pos.x = Game::winWidth / 2.f;
    newPlayer.getComponent<TransformComponent>().pos.y = Game::winHeigth / 2.f;
    newPlayer.addComponent<SpriteComponent>("assets/player.png")
            .setSrcRect({0,0,16,16})
            .addAnimation("idle", {0, 2, 600 })
            .addAnimation("fast", {1, 4, 100 })
            .addAnimation("moving", {1, 4, 200 });
    newPlayer.addComponent<InputComponent>();
    newPlayer.addComponent<ColliderComponent>("player");
    newPlayer.addComponent<WeaponComponent>("shotgun");
    newPlayer.addComponent<RelationshipComponent>();
    newPlayer.addGroup(groupPlayers);

    manager.addEntity().addComponent<FoeSpawnerComponent>(100)
            .addWave({"standard", 10})
            .addWave({"standard", 20})
            .addWave({"standard", 30})
            ;


}

#endif // LEVEL1_H
