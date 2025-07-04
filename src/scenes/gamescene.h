#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "engine/scenemanager.h"
#include "engine/gamemap.h"
#include "ecs/components.h"

class GameScene : public Scene {
public:
    GameScene(SceneManager& sceneManager);
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

    void followPlayer();
};


#endif // GAMESCENE_H