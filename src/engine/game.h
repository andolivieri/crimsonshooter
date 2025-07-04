#ifndef GAME_H
#define GAME_H

#include <set>
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


class Game
{
public:
    Game() = default;
    ~Game() = default;

    void mainLoop();
    void init(const char* title, int xpos, int ypos, int widht, int heigth, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running();


    static SDL_Rect camera;
    static Vector2D cameraToWorld(Vector2D v);
    static Vector2D worldToCamera(Vector2D v);


    static int winWidth;
    static int winHeigth;
    static bool KEYS[322];
    static std::set<SDL_Keycode> pressedKeys;
    static std::set<Uint8> pressedMouseButtons;
    static std::vector<ColliderComponent*> colliders;
    static float deltaTime;
    
    
    static SDL_Renderer* getRenderer();
    
    bool paused();

    
    
private:

    unsigned long cnt = 0;
    bool m_running = false;
    bool m_paused = false;
    SDL_Window* m_win = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    SceneManager* sceneMgr = nullptr;


    void togglePause();
    void dumpStats();
};

#endif // GAME_H
