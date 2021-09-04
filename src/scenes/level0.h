#ifndef LEVEL1_H
#define LEVEL1_H

#include "../game.h"
#include "../gamemap.h"
#include "../ecs.h"
#include "../ecs/components.h"

ScoreData score;

void stuff(EntityManager& manager)
{


    GameMap* g_map = nullptr;


    g_map = new GameMap();

    GameMap::LoadMap("assets/themap.json");

    auto& thePlayer(manager.addEntity());
    thePlayer.addComponent<TransformComponent>(100.f,100.f, 64,64);
    thePlayer.getComponent<TransformComponent>().width = 64;
    thePlayer.getComponent<TransformComponent>().height = 64;
    thePlayer.getComponent<TransformComponent>().pos.x = Game::winWidth / 2.f;
    thePlayer.getComponent<TransformComponent>().pos.y = Game::winHeigth / 2.f;
    thePlayer.addComponent<DamageModelComponent>(200);
    thePlayer.addComponent<RelationshipComponent>();
    thePlayer.addComponent<SpriteComponent>("assets/player.png")
            .setSrcRect({0,0,16,16})
            .addAnimation("idle", {0, 0, 2, 600 })
            .addAnimation("fast", {0, 1, 4, 100 })
            .addAnimation("dead", {0, 2, 1, 100 })
            .addAnimation("moving", {0, 1, 4, 200 });
    thePlayer.addComponent<InputComponent>();
    thePlayer.addComponent<ColliderComponent>("player");
    thePlayer.addComponent<WeaponBayComponent>("uzi")
            .setAttachPoint({8,20});
    thePlayer.addComponent<ActorControllerComponent>();
    thePlayer.addGroup(groupPlayers);

    auto& foespawn = manager.addEntity()
            .addComponent<FoeSpawnerComponent>(score)
            .addWave({"standard", 10, 5})
            .addWave({"standard", 20, 10})
            .addWave({"standard", 30, 15})
            .addWave({"standard", 50, 25})
            ;
    // perkspawner
    manager.addEntity().addComponent<PerkSpawnerComponent>(score);

    // wincondition
   manager.addEntity()
            .addComponent<PredicateComponent>(
                [&](){return foespawn.isFinished();})
            .then([&]()
    {
        auto &e = manager.addEntity();
        e.addComponent<TransformComponent>(1.f, 1.f, 800, 120)
                .centerOn({Game::winWidth/2.f, Game::winHeigth/2.f});
        e.addComponent<TextComponent>("LEVEL 0 COMPLETED");
        e.addGroup(groupOverlay);
    });

   // losecondition
  manager.addEntity()
           .addComponent<PredicateComponent>(
               [&](){return thePlayer.getComponent<DamageModelComponent>().diedNow();})
           .then([&]()
   {
       auto &e = manager.addEntity();
       e.addComponent<TransformComponent>(0.f, 0.f, 500, 150)
               .centerOn({Game::winWidth/2, Game::winHeigth/2});
       e.addComponent<TextComponent>("WASTED");
       e.addGroup(groupOverlay);
   });





}

#endif // LEVEL1_H
