#ifndef GAME_H
#define GAME_H

#include "SDL.h"

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

    static SDL_Event event;
private:
    unsigned long cnt = 0;
    bool m_running = false;
    SDL_Window* m_win = nullptr;
    SDL_Renderer *m_renderer = nullptr;


};

#endif // GAME_H
