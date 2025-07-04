#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <stack>
#include <memory>
#include <string>
#include <functional>
#include <vector>
#include "ecs/ecs.h"

enum class SceneExecutionMode {
    RUN_ALL,        // All loaded scenes run simultaneously
    RUN_TOP_ONLY    // Only the top scene runs, others are paused
};

class Scene {
public:
    Scene(const std::string& name, SceneExecutionMode mode = SceneExecutionMode::RUN_TOP_ONLY);
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

protected:
    EntityManager m_entityManager;
    std::string m_name;
    SceneExecutionMode m_executionMode;
    bool m_paused = false;
    bool m_initialized = false;
};

class SceneManager {
public:
    SceneManager() = default;
    ~SceneManager() = default;
    
    // Scene stack operations
    void pushScene(std::unique_ptr<Scene> scene);
    void popScene();
    void popAllScenes();
    
    // Scene management
    void updateScenes();
    void renderScenes();
    void cleanupScenes();
    
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
    
    void updatePauseStates();
    void executeOnAllScenes(std::function<void(Scene*)> func);
};

#endif // SCENEMANAGER_H