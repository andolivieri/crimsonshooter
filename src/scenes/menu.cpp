#include "engine/game.h"
#include "engine/gamemap.h"
#include <iostream>
#include "menu.h"
#include "gamescene.h"


MenuScene::MenuScene(SceneManager& sceneManager) : 
    Scene("MenuScene", sceneManager, SceneExecutionMode::RUN_TOP_ONLY)
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::init()
{
    std::cout << "Initializing MenuScene" << std::endl;
    createUI();
}

void MenuScene::update()
{
    Scene::update();
}

void MenuScene::render()
{
    Scene::render();
}

void MenuScene::createUI()
{
    auto& mainMenu = m_entityManager.addEntity("main_menu");
    
    int menuWidth = 300;
    int menuHeight = 200;
    int centerX = Game::winWidth / 2 - menuWidth / 2;
    int centerY = Game::winHeigth / 2 - menuHeight / 2;
    
    mainMenu.addComponent<TransformComponent>(centerX, centerY, menuWidth, menuHeight);
    
    auto& menuComp = mainMenu.addComponent<MenuComponent>(menuWidth, menuHeight);
    menuComp.addEntry("Start Game", [this]() {
        m_sceneManager.pushScene(std::make_unique<GameScene>(m_sceneManager));
    });
    
    menuComp.addEntry("Quit", []() {
        SDL_Quit();
        exit(0);
    });
    
    mainMenu.addGroup(groupPlayers);
}