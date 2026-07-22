#ifndef GAME_H
#define GAME_H

#include <set>
#include <string>
#include <vector>
#include "SDL.h"
#include "ecs/components.h"
#include "scenemanager.h"


enum groupLabels
{
    groupFirst,
    groupMap,
    groupColliders,
    groupBloodPatches,
    groupDeadEnemies,
    groupAuras,
    groupFlames,
    groupPerks,
    groupPerksIcon,
    groupWeapons,
    groupPlayers,
    groupEnemies,
    groupProjectiles,
    groupOverlayBg,
    groupOverlay,
    groupLast
};


struct GameLaunchOpts {
    bool windowed = false; // -w --windowed
    bool skipSplash = false; // -s --skip-splash
    bool debugColliders = false; // -d --debug-colliders
    bool showFPS = false; // --fps
    std::string levelId; // --level <id>

    GameLaunchOpts() = default;
};

class Game
{
public:
    Game(GameLaunchOpts);
    ~Game() = default;

    void mainLoop();
    void init(const char* title, int xpos, int ypos, int widht, int heigth, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();
    void renderFPS();

    bool running();


    static SDL_Rect camera;
    static Vector2D cameraToWorld(Vector2D v);
    static Vector2D worldToCamera(Vector2D v);


    static int winWidth;
    static int winHeigth;
    static bool KEYS[322];
    static std::set<SDL_Keycode> pressedKeys;
    static std::set<Uint8> pressedMouseButtons;
    static SDL_GameController* gameController;
    static std::vector<ColliderComponent*> colliders;
    static float deltaTime;
    static bool debugColliders;
    static SceneManager* sceneManager;
    
    
    static SDL_Renderer* getRenderer();
    
    bool paused();

    
    
private:

    GameLaunchOpts m_args;
    unsigned long cnt = 0;
    bool m_running = false;
    bool m_paused = false;
    SDL_Window* m_win = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    SceneManager* sceneMgr = nullptr;
    
    static float fps;
    static int frameCount;
    static uint32_t lastFPSTime;


    void togglePause();
    void dumpStats();
};

#endif // GAME_H
