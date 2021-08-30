#include "Game.h"

int main(int argc, char* argv[])
{
    Game* g = new Game();

    g->init("NiceGame",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1280, 720,
            true
            );

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    uint32_t frameStart;
    int frameTime;

    while(g->running()){
        frameStart = SDL_GetTicks();

        g->handleEvents();
        g->update();
        g->render();

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);

    }

    g->clean();

    return 0;

}
