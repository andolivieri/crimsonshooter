#include "foespawnercomponent.h"
#include "game.h"

void FoeSpawnerComponent::update()
{

    if(waves.empty())
        return;


    FoeWave currentWave = waves.front();
    auto& enemies = entity->m_manager.getGroup(groupEnemies);
    auto currentlyOnScreen = enemies.size();

    while(currentWaveSpawnCount <= currentWave.total && currentlyOnScreen < maxOnScreen)
    {
        spawnFoe(currentWave.whatkind);
        currentlyOnScreen++;
        currentWaveSpawnCount++;
    }

    if(currentWaveSpawnCount >= currentWave.total)
    {
        waves.pop_front();
        currentWaveSpawnCount = 0;
    }



}


void FoeSpawnerComponent::spawnFoe(const std::string foekind)
{

    static long enemyCount = 0;

    auto& theFoe = entity->m_manager.addEntity();
    theFoe.addComponent<TransformComponent>(0.f,0.f, 64,64);
    theFoe.addComponent<SpriteComponent>("assets/foe.png")
            .setSrcRect({0,0,32,32})
            .addAnimation("idle", {0, 1, 100 })
            .addAnimation("dying", {1, 4, 300 })
            .addAnimation("moving", {0, 4, 200 });
    theFoe.addComponent<DamageModelComponent>(30);
    theFoe.addComponent<ColliderComponent>(foekind + std::to_string(enemyCount++), 4, 4, .8f);
    theFoe.addComponent<AIComponent>();
    theFoe.addComponent<RelationshipComponent>();
    theFoe.addGroup(groupEnemies);

    Vector2D spawnPt;

    switch (rand()% 4) {
    case 0:
        // CENTER
        spawnPt.y = -128;
        spawnPt.x = rand() % Game::winWidth;
        break;
    case 1:
        // LEFT
        spawnPt.y = rand() % Game::winHeigth;
        spawnPt.x = 0;
        break;
    case 2:
        // RIGHT
        spawnPt.y = rand() % Game::winHeigth;
        spawnPt.x = Game::winWidth;
        break;
    case 3:
        // BOTTOM
        spawnPt.y = Game::winHeigth;
        spawnPt.x = rand() % Game::winWidth;
        break;
    default:
        break;
    }


    theFoe.getComponent<TransformComponent>().pos = spawnPt;
    float speed = .1f + (rand() / (float)RAND_MAX );
    theFoe.getComponent<AIComponent>().speed = speed;



}
