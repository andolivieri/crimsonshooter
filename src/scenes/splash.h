#ifndef SPLASH_H
#define SPLASH_H

#include "engine/scenemanager.h"
#include "ecs/components.h"

class SplashScene : public Scene {
public:
    SplashScene(SceneManager& sceneManager);
    ~SplashScene() override;

    void init() override;
    void update() override;
    void render() override;

private:
    Entity* m_logoEntity = nullptr;
    Entity* m_textEntity = nullptr;
    Entity* m_timerEntity = nullptr;
    
    void createLogo();
    void createText();
    void createTimer();
};

#endif // SPLASH_H