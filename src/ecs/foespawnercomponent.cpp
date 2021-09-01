#include "foespawnercomponent.h"
#include "game.h"

void FoeSpawnerComponent::update()
{

    finished = (scoreData.kills == totalEnemies);

    if(waves.empty()){
        return;
    }


    FoeWave currentWave = waves.front();
    auto& enemies = entity->m_manager.getGroup(groupEnemies);
    auto currentlyOnScreen = enemies.size();

    while(currentWaveSpawnCount <= currentWave.total && currentlyOnScreen < currentWave.maxOnScreen)
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
    theFoe.addComponent<ScoreCollector>(scoreData);
    theFoe.addComponent<ColliderComponent>(foekind + std::to_string(enemyCount++), 4, 4, .8f);
    theFoe.addComponent<RelationshipComponent>();
    theFoe.addGroup(groupEnemies);

    // Alive aura

    auto& shadow = entity->m_manager.addEntity();
    shadow.addComponent<SpriteComponent>("assets/aura.png")
            .setSrcRect({0,0,32,32})
            .setTransform(&theFoe.getComponent<TransformComponent>())
            .setAlpha(128);
    shadow.addGroup(groupBloodPatches);
    theFoe.getComponent<RelationshipComponent>().addChildren(&shadow, "shadow");


    theFoe.addComponent<AIComponent>();

    Vector2D spawnPt;

    switch (rand()% 4) {
    case 0:
        // CENTER
        spawnPt.y = -128;
        spawnPt.x = static_cast<float>(rand() % Game::winWidth);
        break;
    case 1:
        // LEFT
        spawnPt.y = static_cast<float>(rand() % Game::winHeigth);
        spawnPt.x = 0;
        break;
    case 2:
        // RIGHT
        spawnPt.y = static_cast<float>(rand() % Game::winHeigth);
        spawnPt.x = static_cast<float>(Game::winWidth);
        break;
    case 3:
        // BOTTOM
        spawnPt.y = static_cast<float>(Game::winHeigth);
        spawnPt.x = static_cast<float>(rand() % Game::winWidth);
        break;
    default:
        break;
    }


    theFoe.getComponent<TransformComponent>().pos = spawnPt;
    float speed = .1f + (rand() / (float)RAND_MAX );
    theFoe.getComponent<AIComponent>().speed = speed;



}
