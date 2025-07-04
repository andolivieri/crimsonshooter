#include "gamescene.h"
#include <iostream>
#include "engine/game.h"
#include "engine/gamemap.h"
#include "ecs/ecs.h"
#include "ecs/components.h"

// GameScene implementation
GameScene::GameScene() : Scene("GameScene", SceneExecutionMode::RUN_TOP_ONLY)
{
}

GameScene::~GameScene()
{
    if (m_gameMap)
    {
        delete m_gameMap;
        m_gameMap = nullptr;
    }
}

void GameScene::init()
{
    std::cout << "Initializing GameScene" << std::endl;

    // Load the map
    m_gameMap = new GameMap();

    GameMap::LoadMap("assets/themap.json", m_entityManager);

    auto &thePlayer(m_entityManager.addEntity("player"));
    thePlayer.addComponent<SoundComponent>();
    thePlayer.addComponent<TransformComponent>(100.f, 100.f, 64, 64);
    thePlayer.getComponent<TransformComponent>().width = 64;
    thePlayer.getComponent<TransformComponent>().height = 64;
    thePlayer.getComponent<TransformComponent>().pos.x = Game::winWidth / 2.f;
    thePlayer.getComponent<TransformComponent>().pos.y = Game::winHeigth / 2.f;
    thePlayer.addComponent<DamageModelComponent>(1000);
    thePlayer.addComponent<RelationshipComponent>();
    thePlayer.addComponent<SpriteComponent>("assets/player.png")
        .setSrcRect({0, 0, 16, 16})
        .addAnimation("idle", {0, 0, 2, 600})
        .addAnimation("fast", {0, 1, 4, 100})
        .addAnimation("dead", {0, 2, 1, 100})
        .addAnimation("moving", {0, 1, 4, 150});
    thePlayer.addComponent<InputComponent>();
    thePlayer.addComponent<ColliderComponent>("player");
    thePlayer.addComponent<GrenadeComponent>();
    thePlayer.addComponent<StaminaComponent>(100.0f);

    thePlayer.addComponent<WeaponBayComponent>()
        .setAttachPoint({8, -20}, 0, true)
        .setAttachPoint({8, 20}, 1)
        .equip("handgun", 0);
    thePlayer.addComponent<ActorControllerComponent>();
    thePlayer.tag = "player";
    thePlayer.addGroup(groupPlayers);

    auto &foespawn = m_entityManager.addEntity()
                         .addComponent<FoeSpawnerComponent>(m_score)
                         .addWave({"standard", 10, 5})
                         .addWave({"standard", 20, 10})
                         .addWave({"standard", 30, 15})
                         .addWave({"standard", 50, 25})
                         .addWave({"standard", 120, 35})
                         .addWave({"standard", 1000, 50});
    // perkspawner
    m_entityManager.addEntity().addComponent<PerkSpawnerComponent>(m_score);

    // wincondition
    m_entityManager.addEntity()
        .addComponent<PredicateComponent>(
            [&]()
            {
                return foespawn.allEnemiesDefeated();
            })
        .then([&]()
              {
        auto &e = m_entityManager.addEntity();
        e.addComponent<TransformComponent>(1.f, 1.f, 800, 120)
                .centerOn({Game::winWidth/2.f, Game::winHeigth/2.f});
        e.addComponent<TextComponent>("LEVEL 0 COMPLETED");

        e.addGroup(groupOverlay); });

    // losecondition
    m_entityManager.addEntity()
        .addComponent<PredicateComponent>(
            [&]()
            { return thePlayer.getComponent<DamageModelComponent>().diedNow(); })
        .then([&]()
              {
        auto &e = m_entityManager.addEntity();
        e.addComponent<TransformComponent>(0.f, 0.f, 700, 200)
                .centerOn({Game::winWidth/2, Game::winHeigth/2});
        e.addComponent<TextComponent>("WASTED");
        e.addGroup(groupOverlay); });

    // score
    auto &theOverlay(m_entityManager.addEntity());
    theOverlay.addComponent<OverlayComponent>(m_score, thePlayer);

}

void GameScene::update()
{
    // follow player camera
    followPlayer();
    Scene::update();
}

void GameScene::followPlayer()
{
    if(!m_entityManager.get("player"))
        return;
    Vector2D pos = m_entityManager.get("player")->getComponent<TransformComponent>().pos;
    Game::camera.x = pos.x - Game::winWidth / 2;
    Game::camera.y = pos.y - Game::winHeigth / 2;

    // Apply boundary constraints first
    Game::camera.x = Game::camera.x < 0 ? 0 : Game::camera.x;
    Game::camera.y = Game::camera.y < 0 ? 0 : Game::camera.y;
    Game::camera.x = Game::camera.x + Game::camera.w > GameMap::mapWidth ? GameMap::mapWidth - Game::camera.w : Game::camera.x;
    Game::camera.y = Game::camera.y + Game::camera.h > GameMap::mapHeight ? GameMap::mapHeight - Game::camera.h : Game::camera.y;
}

void GameScene::render()
{
    Scene::render();
}

void GameScene::cleanup()
{
    std::cout << "Cleaning up GameScene" << std::endl;

    if (m_gameMap)
    {
        delete m_gameMap;
        m_gameMap = nullptr;
    }

    Scene::cleanup(); // Call base class cleanup
}

void GameScene::onPush()
{
    std::cout << "GameScene pushed onto stack" << std::endl;
}

void GameScene::onPop()
{
    std::cout << "GameScene popped from stack" << std::endl;
}

void GameScene::onPause()
{
    std::cout << "GameScene paused" << std::endl;
}

void GameScene::onResume()
{
    std::cout << "GameScene resumed" << std::endl;
}