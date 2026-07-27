#include "foespawnercomponent.h"
#include "engine/game.h"
#include "engine/gamemap.h"
#include "helpers/collision.h"
#include "helpers/foeregistry.h"

void FoeSpawnerComponent::update()
{

    finished = (scoreData.kills >= totalEnemies);

    if(waves.empty()){
        return;
    }

    FoeWave currentWave = waves.front();
    auto& enemies = entity->m_manager.getGroup(groupEnemies);
    auto currentlyOnScreen = enemies.size();

    while(currentWaveSpawnCount < currentWave.total && currentlyOnScreen < currentWave.maxOnScreen)
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

bool FoeSpawnerComponent::allEnemiesDefeated() const
{
    return (scoreData.kills >= totalEnemies);
}

void FoeSpawnerComponent::spawnFoe(const std::string foekind)
{

    static long enemyCount = 0;
    const long foeId = enemyCount++;

    auto& theFoe = entity->m_manager.addEntity("foe" + std::to_string(foeId));
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

    const FoeKindData& kind = FoeRegistry::get(foekind);

    theFoe.getComponent<TransformComponent>().pos = spawnPt;
    theFoe.addComponent<SpriteComponent>(kind.sprite)
            .setSrcRect(kind.srcRect)
            .addAnimation("moving", {kind.moving.col, kind.moving.row, kind.moving.frames, kind.moving.speed})
            .addAnimation("dying", {kind.dying.col, kind.dying.row, kind.dying.frames, kind.dying.speed})
            .addAnimation("idle", {kind.idle.col, kind.idle.row, kind.idle.frames, kind.idle.speed})
            .addAnimation("critical", {kind.critical.col, kind.critical.row, kind.critical.frames, kind.critical.speed});
    theFoe.addComponent<DamageModelComponent>(kind.health);
    theFoe.addComponent<ScoreCollector>(scoreData);
    const int contactDamage = kind.contactDamage;
    theFoe.addComponent<ColliderComponent>(foekind + std::to_string(foeId), 4, 4, .8f)
            .setBlockedBySolids("!PERIMETER")
            .setCircle()
            .onCollision([&theFoe, contactDamage](Entity& target){

        Entity* player = theFoe.m_manager.get("player");

        if(&target == player){
            player->getComponent<DamageModelComponent>().health -= contactDamage;
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
            .setAlpha(128)
            .setColorMod(
                kind.colorModifier.r, 
                kind.colorModifier.g, 
                kind.colorModifier.b
            );
    shadow.addGroup(groupAuras);
    theFoe.getComponent<RelationshipComponent>().addChildren(&shadow, "shadow");


    theFoe.addComponent<PathfindingComponent>();
    theFoe.addComponent<AIComponent>();
    float speed = kind.speedMin + (rand() / (float)RAND_MAX) * (kind.speedMax - kind.speedMin);
    theFoe.getComponent<AIComponent>().speed = speed;



}
