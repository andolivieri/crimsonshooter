#include "scenemanager.h"
#include <iostream>
#include <vector>
#include <SDL_render.h>
#include <algorithm>
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
    
    updateFade();
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
    
    renderFadeOverlay();
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

void Scene::startFadeIn(float duration)
{
    if (!m_fadeEnabled) {
        m_fadeState = FadeState::VISIBLE;
        m_fadeAlpha = 1.0f;
        return;
    }
    
    m_fadeState = FadeState::FADE_IN;
    m_fadeDuration = duration;
    m_fadeTimer = 0.0f;
    m_fadeAlpha = 0.0f;
}

void Scene::startFadeOut(float duration)
{
    if (!m_fadeEnabled) {
        m_fadeState = FadeState::NONE;
        m_fadeAlpha = 0.0f;
        return;
    }
    
    m_fadeState = FadeState::FADE_OUT;
    m_fadeDuration = duration;
    m_fadeTimer = 0.0f;
    m_fadeAlpha = 1.0f;
}

bool Scene::isFadeComplete() const
{
    return m_fadeState == FadeState::VISIBLE || m_fadeState == FadeState::NONE;
}

void Scene::updateFade()
{
    if (m_fadeState == FadeState::NONE || m_fadeState == FadeState::VISIBLE) {
        return;
    }
    
    m_fadeTimer += Game::deltaTime;
    
    if (m_fadeTimer >= m_fadeDuration) {
        if (m_fadeState == FadeState::FADE_IN) {
            m_fadeState = FadeState::VISIBLE;
            m_fadeAlpha = 1.0f;
        } else if (m_fadeState == FadeState::FADE_OUT) {
            m_fadeState = FadeState::NONE;
            m_fadeAlpha = 0.0f;
        }
        m_fadeTimer = 0.0f;
    } else {
        float t = m_fadeTimer / m_fadeDuration;
        if (m_fadeState == FadeState::FADE_IN) {
            m_fadeAlpha = t;
        } else if (m_fadeState == FadeState::FADE_OUT) {
            m_fadeAlpha = 1.0f - t;
        }
    }
}

void Scene::renderFadeOverlay()
{
    if (!m_fadeEnabled || m_fadeState == FadeState::VISIBLE) {
        return;
    }
    
    SDL_Renderer* renderer = Game::getRenderer();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    Uint8 alpha = static_cast<Uint8>((1.0f - m_fadeAlpha) * 255);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
    
    SDL_Rect fullscreen = {0, 0, Game::winWidth, Game::winHeigth};
    SDL_RenderFillRect(renderer, &fullscreen);
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void SceneManager::pushScene(std::unique_ptr<Scene> scene, const SceneTransitionParams& params)
{
    if (!scene) return;
    
    if (m_processingScenes) {
        DeferredSceneCommand cmd;
        cmd.command = SceneCommand::PUSH_SCENE;
        cmd.scene = std::move(scene);
        cmd.params = params;
        m_commandQueue.push(std::move(cmd));
    } else {
        pushSceneImmediate(std::move(scene), params);
    }
}

void SceneManager::pushSceneImmediate(std::unique_ptr<Scene> scene, const SceneTransitionParams& params)
{
    if (!scene) return;
    
    std::cout << "Pushing scene: " << scene->getName() << std::endl;
    
    // fade out current scene
    if (!m_sceneStack.empty() && !params.skipFade) {
        Scene* currentScene = m_sceneStack.top().get();
        if (currentScene->isFadeEnabled()) {
            currentScene->startFadeOut(params.fadeOutDuration);
        }
    }
    
    scene->onPush();
    
    // fade in on new one
    if (!params.skipFade) {
        scene->startFadeIn(params.fadeInDuration);
    } else {
        scene->setFadeEnabled(false);
    }
    
    m_sceneStack.push(std::move(scene));
    updatePauseStates();
}

void SceneManager::popScene(const SceneTransitionParams& params)
{
    if (m_sceneStack.empty()) return;
    
    if (m_processingScenes) {
        DeferredSceneCommand cmd;
        cmd.command = SceneCommand::POP_SCENE;
        cmd.params = params;
        m_commandQueue.push(std::move(cmd));
    } else {
        popSceneImmediate(params);
    }
}

void SceneManager::popSceneImmediate(const SceneTransitionParams& params)
{
    if (m_sceneStack.empty()) return;
    
    auto& topScene = m_sceneStack.top();
    std::cout << "Popping scene: " << topScene->getName() << std::endl;
    
    // fade out on current scene
    if (!params.skipFade && topScene->isFadeEnabled()) {
        topScene->startFadeOut(params.fadeOutDuration);
    }
    
    topScene->onPop();
    topScene->cleanup();
    m_sceneStack.pop();
    
    // fade in the next one
    if (!m_sceneStack.empty() && !params.skipFade) {
        Scene* newCurrentScene = m_sceneStack.top().get();
        if (newCurrentScene->isFadeEnabled()) {
            newCurrentScene->startFadeIn(params.fadeInDuration);
        }
    }
    
    updatePauseStates();
}

void SceneManager::popAllScenes()
{
    if (m_processingScenes) {
        DeferredSceneCommand cmd;
        cmd.command = SceneCommand::POP_ALL_SCENES;
        m_commandQueue.push(std::move(cmd));
    } else {
        popAllScenesImmediate();
    }
}

void SceneManager::popAllScenesImmediate()
{
    while (!m_sceneStack.empty()) {
        popSceneImmediate({true});
    }
}

void SceneManager::updateScenes()
{
    if (m_sceneStack.empty()) return;
    
    m_processingScenes = true;
    
    if (m_globalExecutionMode == SceneExecutionMode::RUN_ALL) {
        // Update all scenes from bottom to top
        executeOnAllScenes([](Scene* scene) {
            scene->update();
        });
    } else {
        // Update only the top scene
        m_sceneStack.top()->update();
    }
    
    m_processingScenes = false;
}

void SceneManager::renderScenes()
{
    if (m_sceneStack.empty()) return;
    
    m_processingScenes = true;
    
    if (m_globalExecutionMode == SceneExecutionMode::RUN_ALL) {
        // Render all scenes from bottom to top
        executeOnAllScenes([](Scene* scene) {
            scene->render();
        });
    } else {
        // Render only the top scene
        m_sceneStack.top()->render();
    }
    
    m_processingScenes = false;
}

void SceneManager::processDeferredCommands()
{
    while (!m_commandQueue.empty()) {
        auto& cmd = m_commandQueue.front();
        
        switch (cmd.command) {
            case SceneCommand::PUSH_SCENE:
                pushSceneImmediate(std::move(cmd.scene), cmd.params);
                break;
            case SceneCommand::POP_SCENE:
                popSceneImmediate(cmd.params);
                break;
            case SceneCommand::POP_ALL_SCENES:
                popAllScenesImmediate();
                break;
        }
        
        m_commandQueue.pop();
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
    
    while (!m_sceneStack.empty()) {
        sceneOrder.push_back(m_sceneStack.top().get());
        tempStack.push(std::move(m_sceneStack.top()));
        m_sceneStack.pop();
    }
    
    while (!tempStack.empty()) {
        m_sceneStack.push(std::move(tempStack.top()));
        tempStack.pop();
    }
    
    for (Scene* scene : sceneOrder) {
        func(scene);
    }
}