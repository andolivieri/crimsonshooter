#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <stack>
#include <memory>
#include <string>
#include <functional>
#include <vector>
#include <queue>
#include <SDL.h>
#include "ecs/ecs.h"

class Scene;

enum class SceneExecutionMode {
    RUN_ALL,        // All loaded scenes run simultaneously
    RUN_TOP_ONLY    // Only the top scene runs, others are paused
};

enum class FadeState {
    NONE,      
    FADE_IN,
    FADE_OUT,
    VISIBLE
};

struct SceneTransitionParams {
    bool skipFade = false;
    float fadeInDuration = 0.5f;
    float fadeOutDuration = 0.5f;
};

enum class SceneCommand {
    PUSH_SCENE,
    POP_SCENE,
    POP_ALL_SCENES
};

struct DeferredSceneCommand {
    SceneCommand command;
    std::unique_ptr<Scene> scene;  // Only used for PUSH_SCENE
    SceneTransitionParams params;
};

class SceneManager; // Forward declaration

class Scene {
public:
    Scene(const std::string& name, SceneManager& sceneManager, SceneExecutionMode mode = SceneExecutionMode::RUN_TOP_ONLY);
    virtual ~Scene();

    // Scene lifecycle methods
    virtual void init() = 0;
    virtual void update();
    virtual void render();
    virtual void cleanup();
    
    // Scene state management
    void pause();
    void resume();
    bool isPaused() const { return m_paused; }
    
    // Getters
    const std::string& getName() const { return m_name; }
    SceneExecutionMode getExecutionMode() const { return m_executionMode; }
    EntityManager& getEntityManager() { return m_entityManager; }
    
    // Scene transition callbacks
    virtual void onPush() {}    // Called when scene is pushed onto stack
    virtual void onPop() {}     // Called when scene is popped from stack
    virtual void onPause() {}   // Called when scene is paused
    virtual void onResume() {}  // Called when scene is resumed
    
    // Fade transition methods
    void startFadeIn(float duration = 0.5f);
    void startFadeOut(float duration = 0.5f);
    void setFadeEnabled(bool enabled) { m_fadeEnabled = enabled; }
    bool isFadeEnabled() const { return m_fadeEnabled; }
    FadeState getFadeState() const { return m_fadeState; }
    bool isFadeComplete() const;
    float getFadeAlpha() const { return m_fadeAlpha; }
    void setClearColor(SDL_Color color) { m_clearColor = color; }

protected:
    EntityManager m_entityManager;
    SceneManager& m_sceneManager;
    std::string m_name;
    SceneExecutionMode m_executionMode;
    bool m_paused = false;
    bool m_initialized = false;

    // Default background
    SDL_Color m_clearColor = {59, 55, 61, 255};
    
    // Fade transition properties
    bool m_fadeEnabled = true;
    FadeState m_fadeState = FadeState::NONE;
    float m_fadeAlpha = 1.0f;
    float m_fadeDuration = 0.5f;
    float m_fadeTimer = 0.0f;
    
    void updateFade();
    void renderFadeOverlay();
};

class SceneManager {
public:
    SceneManager() = default;
    ~SceneManager() = default;
    
    void pushScene(std::unique_ptr<Scene> scene, const SceneTransitionParams& params = {});
    void popScene(const SceneTransitionParams& params = {});
    void popAllScenes();
    
    // Scene management
    void updateScenes();
    void renderScenes();
    void cleanupScenes();
    
    void processDeferredCommands();
    
    // Getters
    Scene* getCurrentScene() const;
    Scene* getScene(const std::string& name) const;
    size_t getSceneCount() const { return m_sceneStack.size(); }
    bool isEmpty() const { return m_sceneStack.empty(); }
    
    // Utility
    void setGlobalExecutionMode(SceneExecutionMode mode) { m_globalExecutionMode = mode; }
    SceneExecutionMode getGlobalExecutionMode() const { return m_globalExecutionMode; }

private:
    std::stack<std::unique_ptr<Scene>> m_sceneStack;
    SceneExecutionMode m_globalExecutionMode = SceneExecutionMode::RUN_TOP_ONLY;
    
    // Deferred commands
    std::queue<DeferredSceneCommand> m_commandQueue;
    bool m_processingScenes = false;
    
    void updatePauseStates();
    void executeOnAllScenes(std::function<void(Scene*)> func);
    
    void pushSceneImmediate(std::unique_ptr<Scene> scene, const SceneTransitionParams& params = {});
    void popSceneImmediate(const SceneTransitionParams& params = {});
    void popAllScenesImmediate();
};

#endif // SCENEMANAGER_H