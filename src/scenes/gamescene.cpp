#include "gamescene.h"
#include <cstdlib>
#include <iostream>
#include "engine/game.h"
#include "engine/gamemap.h"
#include "engine/levelmanager.h"
#include "ecs/ecs.h"
#include "ecs/components.h"

GameScene::GameScene(SceneManager& sceneManager, std::string levelId)
    : Scene("GameScene", sceneManager, SceneExecutionMode::RUN_TOP_ONLY),
      m_levelId(std::move(levelId))
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
    std::cout << "Initializing GameScene: " << m_levelId << std::endl;

    LevelManager::load();
    const LevelData* level = LevelManager::get(m_levelId);

    // Load the map named by the level manifest (fall back to the legacy map).
    m_gameMap = new GameMap();

    const std::string mapPath =
        (level && !level->mapPath.empty()) ? level->mapPath : "assets/themap.json";
    GameMap::LoadMap(mapPath, m_entityManager);

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
    thePlayer.addComponent<ColliderComponent>("player").setBlockedBySolids().setCircle();
    thePlayer.addComponent<GrenadeComponent>();
    thePlayer.addComponent<StaminaComponent>(100.0f);

    thePlayer.addComponent<WeaponBayComponent>()
        .setAttachPoint({8, -20}, 0, true)
        .setAttachPoint({8, 20}, 1)
        .equip("handgun", 0);
    thePlayer.addComponent<ActorControllerComponent>();
    thePlayer.tag = "player";
    thePlayer.addGroup(groupPlayers);

    auto &foespawn = m_entityManager.addEntity().addComponent<FoeSpawnerComponent>(m_score);
    if (level && !level->waves.empty())
    {
        for (const auto &w : level->waves)
        {
            foespawn.addWave(w);
        }
    }
    else
    {
        // Fallback
        foespawn.addWave({"standard", 10, 5})
            .addWave({"standard", 20, 10})
            .addWave({"standard", 30, 15})
            .addWave({"standard", 50, 25})
            .addWave({"standard", 120, 35})
            .addWave({"standard", 1000, 50});
    }

    registerActions(foespawn);
    spawnTriggerZones();

    // perkspawner
    m_entityManager.addEntity().addComponent<PerkSpawnerComponent>(
        m_score,
        level ? level->startingPerks : std::vector<std::string>{},
        level ? level->perks : PerkPoolConfig{});

    // wincondition
    FoeSpawnerComponent *spawner = &foespawn;
    m_entityManager.addEntity()
        .addComponent<PredicateComponent>(
            [spawner]()
            {
                return spawner->allEnemiesDefeated();
            })
        .then([this]()
              { onLevelComplete(); });

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

void GameScene::registerActions(FoeSpawnerComponent& spawner)
{
    FoeSpawnerComponent* sp = &spawner;

    // Generic action: spawn a wave. param format "kind:count[:maxOnScreen]".
    m_actions.reg("spawn_wave", [sp](const std::string& param)
    {
        std::string kind = "standard";
        int count = 5;
        int maxOnScreen = 5;

        const auto firstSep = param.find(':');
        if (firstSep == std::string::npos)
        {
            if (!param.empty())
            {
                kind = param;
            }
        }
        else
        {
            kind = param.substr(0, firstSep);
            const std::string rest = param.substr(firstSep + 1);
            const auto secondSep = rest.find(':');
            if (secondSep == std::string::npos)
            {
                count = std::atoi(rest.c_str());
                maxOnScreen = count;
            }
            else
            {
                count = std::atoi(rest.substr(0, secondSep).c_str());
                maxOnScreen = std::atoi(rest.substr(secondSep + 1).c_str());
            }
        }

        std::cout << "action spawn_wave: " << kind << " x" << count
                  << " (max " << maxOnScreen << ")" << std::endl;
        sp->addWave({kind, count, maxOnScreen});
    });

    // Generic action: pop up an overlay message (the param is the text).
    m_actions.reg("show_message", [this](const std::string& param)
    {
        auto& e = m_entityManager.addEntity();
        e.addComponent<TransformComponent>(1.f, 1.f, 800, 120)
            .centerOn({Game::winWidth / 2.f, Game::winHeigth / 2.f});
        e.addComponent<TextComponent>(param);
        e.addGroup(groupOverlay);
    });

    // Example one-off
    m_actions.reg("test", [](const std::string&)
    {
        std::cout << "test action fired!" << std::endl;
    });
}

void GameScene::spawnTriggerZones()
{
    for (const auto& obj : GameMap::objects)
    {
        // A trigger is any object typed "trigger" or carrying an "action" prop.
        if (obj.type != "trigger" && !obj.hasProp("action"))
        {
            continue;
        }

        const std::string action = obj.getString("action");
        if (action.empty())
        {
            continue;
        }
        const std::string param = obj.getString("param");
        const bool oneShot = obj.getBool("oneShot", true);

        std::cout << "Trigger zone '" << obj.name << "' -> " << action
                  << " (" << param << ")" << std::endl;

        m_entityManager.addEntity()
            .addComponent<TriggerZoneComponent>(obj.rect, action, param, m_actions, oneShot);
    }
}

void GameScene::onLevelComplete()
{
    const LevelData* current = LevelManager::get(m_levelId);
    const LevelData* nextLevel = LevelManager::next(m_levelId);
    const std::string name = current ? current->name : m_levelId;

    auto& banner = m_entityManager.addEntity();
    banner.addComponent<TransformComponent>(1.f, 1.f, 800, 120)
        .centerOn({Game::winWidth / 2.f, Game::winHeigth / 2.f});
    banner.addComponent<TextComponent>(nextLevel ? (name + " COMPLETED") : "THE END.");
    banner.addGroup(groupOverlay);

    if (!nextLevel)
    {
        return;
    }

    const std::string nextId = nextLevel->id;
    const uint32_t startTick = SDL_GetTicks();
    m_entityManager.addEntity()
        .addComponent<PredicateComponent>(
            [startTick]()
            { return SDL_GetTicks() - startTick > 2500; })
        .then([this, nextId]()
        {
            m_sceneManager.popAllScenes();
            m_sceneManager.pushScene(std::make_unique<GameScene>(m_sceneManager, nextId));
        });
}

void GameScene::update()
{
    // follow player camera
    followPlayer();
    Scene::update();
    StickerBaker::flush(Game::getRenderer(), GameMap::staticLayerTexture);
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