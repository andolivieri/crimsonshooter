#include "scenemanager.h"
#include <iostream>
#include <vector>
#include <SDL_render.h>
#include "game.h"

// Scene implementation
Scene::Scene(const std::string& name, SceneManager& sceneManager, SceneExecutionMode mode)
    : m_name(name), m_sceneManager(sceneManager), m_executionMode(mode)
{
}

Scene::~Scene()
{
    cleanup();
}

void Scene::update()
{
    if (m_paused) return;
    
    if (!m_initialized) {
        init();
        m_initialized = true;
    }
    
    m_entityManager.update();
    m_entityManager.refresh();
}

void Scene::render()
{
    if (m_paused && m_executionMode == SceneExecutionMode::RUN_TOP_ONLY) return;
    
    SDL_RenderClear(Game::getRenderer());
    // TODO andoli: groups should be defined per scene
    // but its too much refactor now as many components use them
    for(int g = 0; g != groupLast; g++)
    {
        auto& entities = m_entityManager.getGroup(g);
        for(auto i{0}; i < entities.size(); i++)
        {
            entities[i]->draw();
        }
    }

    SDL_RenderPresent(Game::getRenderer());
}

void Scene::cleanup()
{
    m_entityManager.clear();
    m_initialized = false;
}

void Scene::pause()
{
    if (!m_paused) {
        m_paused = true;
        onPause();
    }
}

void Scene::resume()
{
    if (m_paused) {
        m_paused = false;
        onResume();
    }
}

void SceneManager::pushScene(std::unique_ptr<Scene> scene)
{
    if (!scene) return;
    
    std::cout << "Pushing scene: " << scene->getName() << std::endl;
    
    scene->onPush();
    m_sceneStack.push(std::move(scene));
    updatePauseStates();
}

void SceneManager::popScene()
{
    if (m_sceneStack.empty()) return;
    
    auto& topScene = m_sceneStack.top();
    std::cout << "Popping scene: " << topScene->getName() << std::endl;
    
    topScene->onPop();
    topScene->cleanup();
    m_sceneStack.pop();
    
    updatePauseStates();
}

void SceneManager::popAllScenes()
{
    while (!m_sceneStack.empty()) {
        popScene();
    }
}

void SceneManager::updateScenes()
{
    if (m_sceneStack.empty()) return;
    
    if (m_globalExecutionMode == SceneExecutionMode::RUN_ALL) {
        // Update all scenes from bottom to top
        executeOnAllScenes([](Scene* scene) {
            scene->update();
        });
    } else {
        // Update only the top scene
        m_sceneStack.top()->update();
    }
}

void SceneManager::renderScenes()
{
    if (m_sceneStack.empty()) return;
    
    if (m_globalExecutionMode == SceneExecutionMode::RUN_ALL) {
        // Render all scenes from bottom to top
        executeOnAllScenes([](Scene* scene) {
            scene->render();
        });
    } else {
        // Render only the top scene
        m_sceneStack.top()->render();
    }
}

void SceneManager::cleanupScenes()
{
    while (!m_sceneStack.empty()) {
        m_sceneStack.top()->cleanup();
        m_sceneStack.pop();
    }
}

Scene* SceneManager::getCurrentScene() const
{
    if (m_sceneStack.empty()) return nullptr;
    return m_sceneStack.top().get();
}

Scene* SceneManager::getScene(const std::string& name) const
{
    if (m_sceneStack.empty()) return nullptr;
    
    Scene* foundScene = nullptr;
    
    // Use a const_cast to temporarily modify the stack for searching
    // This is safe because we restore it immediately
    auto& nonConstStack = const_cast<std::stack<std::unique_ptr<Scene>>&>(m_sceneStack);
    std::stack<std::unique_ptr<Scene>> tempStack;
    
    // Move scenes to temp stack while searching
    while (!nonConstStack.empty()) {
        Scene* scene = nonConstStack.top().get();
        if (scene->getName() == name) {
            foundScene = scene;
        }
        tempStack.push(std::move(nonConstStack.top()));
        nonConstStack.pop();
    }
    
    // Restore the original stack
    while (!tempStack.empty()) {
        nonConstStack.push(std::move(tempStack.top()));
        tempStack.pop();
    }
    
    return foundScene;
}

void SceneManager::updatePauseStates()
{
    if (m_sceneStack.empty()) return;
    
    if (m_globalExecutionMode == SceneExecutionMode::RUN_TOP_ONLY) {
        // Pause all scenes except the top one
        bool isTopScene = true;
        executeOnAllScenes([&isTopScene](Scene* scene) {
            if (isTopScene) {
                scene->resume(); // Top scene
                isTopScene = false;
            } else {
                scene->pause();  // All other scenes
            }
        });
    } else {
        // Resume all scenes
        executeOnAllScenes([](Scene* scene) {
            scene->resume();
        });
    }
}

void SceneManager::executeOnAllScenes(std::function<void(Scene*)> func)
{
    if (m_sceneStack.empty()) return;
    
    std::stack<std::unique_ptr<Scene>> tempStack;
    std::vector<Scene*> sceneOrder;
    
    // Move all scenes to temp stack and collect pointers in reverse order
    while (!m_sceneStack.empty()) {
        sceneOrder.push_back(m_sceneStack.top().get());
        tempStack.push(std::move(m_sceneStack.top()));
        m_sceneStack.pop();
    }
    
    // Restore the original stack
    while (!tempStack.empty()) {
        m_sceneStack.push(std::move(tempStack.top()));
        tempStack.pop();
    }
    
    // Execute function on scenes in the correct order (top to bottom)
    for (Scene* scene : sceneOrder) {
        func(scene);
    }
}