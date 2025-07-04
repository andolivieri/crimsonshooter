#ifndef MENU_H
#define MENU_H

#include "engine/scenemanager.h"
#include "engine/gamemap.h"
#include "ecs/components.h"

class MenuScene : public Scene {
public:
    MenuScene();
    ~MenuScene() override;

    void init() override;
    void update() override;
    void render() override;

private:
    void createUI();
};

#endif // MENU_H