#include "foespawnercomponent.h"
#include "game.h"
#include "gamemap.h"
#include "collision.h"

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

    Vector2D spawnPt;

    switch (rand()% 4) {
    case 0:
        // CENTER
        spawnPt.y = -128;
        spawnPt.x = static_cast<float>(rand() % GameMap::mapWidth);
        break;
    case 1:
        // LEFT
        spawnPt.y = static_cast<float>(rand() % GameMap::mapHeight);
        spawnPt.x = 0;
        break;
    case 2:
        // RIGHT
        spawnPt.y = static_cast<float>(rand() % GameMap::mapHeight);
        spawnPt.x = static_cast<float>(GameMap::mapWidth);
        break;
    case 3:
        // BOTTOM
        spawnPt.y = static_cast<float>(GameMap::mapHeight);
        spawnPt.x = static_cast<float>(rand() % GameMap::mapWidth);
        break;
    default:
        break;
    }

    theFoe.getComponent<TransformComponent>().pos = spawnPt;
    theFoe.addComponent<SpriteComponent>("assets/foe.png")
            .setSrcRect({0,0,32,32})
            .addAnimation("dying", {0, 1, 4, 300 })
            .addAnimation("moving", {0, 0, 4, 200 })
            .addAnimation("idle", {0, 2, 2, 500 });
    theFoe.addComponent<DamageModelComponent>(30);
    theFoe.addComponent<ScoreCollector>(scoreData);
    theFoe.addComponent<ColliderComponent>(foekind + std::to_string(enemyCount++), 4, 4, .8f)
            .onCollision([&](Entity& target){

        Entity* player = theFoe.m_manager.get("player");

        if(&target == player){
            player->getComponent<DamageModelComponent>().health -= 10;
        }

    });
    theFoe.addComponent<RelationshipComponent>();
    theFoe.addGroup(groupEnemies);

    // Alive aura

    auto& shadow = entity->m_manager.addEntity();
    shadow.addComponent<TransformComponent>(theFoe.getComponent<TransformComponent>());
    shadow.addComponent<SpriteComponent>("assets/aura.png")
            .setSrcRect({0,0,32,32})
            .setTransform(&theFoe.getComponent<TransformComponent>())
            .setAlpha(128);
    shadow.addGroup(groupAuras);
    theFoe.getComponent<RelationshipComponent>().addChildren(&shadow, "shadow");


    theFoe.addComponent<AIComponent>();
    float speed = .5f + (rand() / (float)RAND_MAX );
    theFoe.getComponent<AIComponent>().speed = speed;



}
