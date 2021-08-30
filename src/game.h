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
    groupPlayers,
    groupEnemies,
    groupColliders,
    groupProjectiles,
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

    static bool KEYS[322];
    static std::set<SDL_Keycode> pressedKeys;
    static std::set<Uint8> pressedMouseButtons;
    static std::vector<ColliderComponent*> colliders;
    static void addTile(SDL_Texture *sdlTexture, const SDL_Rect &src, const SDL_Rect &dst, SDL_RendererFlip flip);
private:
    unsigned long cnt = 0;
    bool m_running = false;
    SDL_Window* m_win = nullptr;
    SDL_Renderer *m_renderer = nullptr;

    int winWidth;
    int winHeigth;


    void spawnFoe();
};

#endif // GAME_H
