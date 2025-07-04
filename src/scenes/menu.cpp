#include "engine/game.h"
#include "engine/gamemap.h"
#include <iostream>
#include "menu.h"


// MenuScene implementation
MenuScene::MenuScene() : 
    Scene("MenuScene", SceneExecutionMode::RUN_TOP_ONLY)
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
    // UI-specific update logic
}

void MenuScene::render()
{
    Scene::render();
    // Additional UI rendering
}

void MenuScene::createUI()
{
    // Create a red square in the center of the screen
    auto& redSquare = m_entityManager.addEntity("redsquare");
    
    // Position the square in the center of the screen
    // Square size: 100x100 pixels
    int squareSize = 100;
    int centerX = Game::winWidth / 2 - squareSize / 2;
    int centerY = Game::winHeigth / 2 - squareSize / 2;
    
    redSquare.addComponent<TransformComponent>(centerX, centerY, squareSize, squareSize);
    
    // Create a red colored sprite component
    // We'll use a simple colored rectangle by creating a solid color texture
    redSquare.addComponent<SpriteComponent>("assets/player.png") // Use existing texture as base
        .setSrcRect({0, 0, 16, 16}); // Use a small part of the texture
        //.setColorMod(255, 0, 0); // Make it red
    
    // Make it absolute positioning so it doesn't move with camera
    redSquare.getComponent<SpriteComponent>().setAbsolute(true);
    
    // Add to UI group for rendering order
    redSquare.addGroup(groupPlayers);

}