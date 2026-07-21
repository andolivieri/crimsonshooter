#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "engine/scenemanager.h"
#include "engine/gamemap.h"
#include "engine/actionregistry.h"
#include "ecs/components.h"

class GameScene : public Scene {
public:
    GameScene(SceneManager& sceneManager, std::string levelId = "level0");
    ~GameScene() override;

    void init() override;
    void update() override;
    void render() override;
    void cleanup() override;

    // Scene transition callbacks
    void onPush() override;
    void onPop() override;
    void onPause() override;
    void onResume() override;

private:
    GameMap* m_gameMap = nullptr;
    ScoreData m_score;
    std::string m_levelId;
    ActionRegistry m_actions;

    void registerActions(FoeSpawnerComponent& spawner);
    void spawnTriggerZones();
    void onLevelComplete();
    void followPlayer();
};


#endif // GAMESCENE_H