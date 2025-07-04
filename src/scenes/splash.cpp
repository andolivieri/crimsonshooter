#include "splash.h"
#include "menu.h"
#include "engine/game.h"
#include <iostream>

SplashScene::SplashScene(SceneManager& sceneManager) : 
    Scene("SplashScene", sceneManager, SceneExecutionMode::RUN_TOP_ONLY)
{
}

SplashScene::~SplashScene()
{
}

void SplashScene::init()
{
    std::cout << "Initializing SplashScene" << std::endl;
    
    // Set dark blue background by clearing with that color
    SDL_SetRenderDrawColor(Game::getRenderer(), 10, 10, 30, 255);
    
    createLogo();
    createText();
    createTimer();
}

void SplashScene::update()
{
    Scene::update();
}

void SplashScene::render()
{
    Scene::render();
}

void SplashScene::createLogo()
{
    auto& logoEntity = m_entityManager.addEntity("logo");
    
    // Center the logo on screen
    int logoWidth = 512;
    int logoHeight = 512;
    int centerX = Game::winWidth / 2 - logoWidth / 2;
    int centerY = Game::winHeigth / 2 - logoHeight / 2 - 50; // Offset up to make room for text
    
    logoEntity.addComponent<TransformComponent>(centerX, centerY, logoWidth, logoHeight);
    logoEntity.addComponent<SpriteComponent>("assets/logo.png")
        .setSrcRect({0,0,128,128})
        .setAbsolute(true);
    
    logoEntity.addGroup(groupOverlay);
    
    m_logoEntity = &logoEntity;
}

void SplashScene::createText()
{
    auto& textEntity = m_entityManager.addEntity("company_text");
    
    // Position text below the logo
    int textX = Game::winWidth / 2 - 250; // Rough centering
    int textY = Game::winHeigth / 2 + 100;
    
    textEntity.addComponent<TransformComponent>(textX, textY, 500, 32);
    textEntity.addComponent<TextComponent>("mansarda games")
        .setAbsolute(true)
        .setColor({255, 255, 255, 255});
    
    textEntity.addGroup(groupOverlay);
    
    m_textEntity = &textEntity;
}

void SplashScene::createTimer()
{
    auto& timerEntity = m_entityManager.addEntity("splash_timer");
    
    timerEntity.addComponent<TimerComponent>(3500, false)
        .onTrigger([this](Entity& _) {
            m_sceneManager.popScene({false, 2.0, 2.0});
            m_sceneManager.pushScene(std::make_unique<MenuScene>(m_sceneManager), 
                                   {false, 1.0, 3.0});
        });
    
    m_timerEntity = &timerEntity;
}