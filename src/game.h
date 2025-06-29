#ifndef GAME_H
#define GAME_H

#include <set>
#include <vector>
#include "SDL.h"
#include "ecs/components.h"

enum groupLabels
{
    groupMap,
    groupColliders,
    groupBloodPatches,
    groupDeadEnemies,
    groupAuras,
    groupPerks,
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
    Game();
    ~Game();

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
    static void addTile(SDL_Texture *sdlTexture, const SDL_Rect &src, const SDL_Rect &dst, SDL_RendererFlip flip);
    bool paused();
private:

    unsigned long cnt = 0;
    bool m_running = false;
    bool m_paused = false;
    SDL_Window* m_win = nullptr;
    SDL_Renderer *m_renderer = nullptr;


    void togglePause();
    void dumpStats();
};

#endif // GAME_H
