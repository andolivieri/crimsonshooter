#include "Game.h"
#include "utils.h"
int main(int argc, char* argv[])
{
    Game* g = new Game();
    ENG_UNUSED(argc);
    ENG_UNUSED(argv);

    g->init("NiceGame",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1280, 720,
            false
            );

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    uint32_t frameStart;
    int frameTime;

    while(g->running()){
        frameStart = SDL_GetTicks();

        g->handleEvents();
        if(!g->paused()){
            g->update();
            g->render();
        }

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);

    }

    g->clean();

    return 0;

}
