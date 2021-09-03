#ifndef GAME_H
#define GAME_H

#include <set>
#include <vector>
#include "SDL.h"
#include "ecs/components.h"

enum groupLabels
{
    groupMap,
    groupBloodPatches,
    groupDeadEnemies,
    groupPerks,
    groupWeapons,
    groupPlayers,
    groupEnemies,
    groupColliders,
    groupProjectiles,
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



    void spawnFoe();
    void togglePause();
};

#endif // GAME_H
