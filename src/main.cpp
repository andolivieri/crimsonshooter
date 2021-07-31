#include "Game.h"

int main(int argc, char* argv[])
{
    Game* g = new Game();

    g->init("NiceGame",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            800, 600,
            false
            );

    while(g->running()){
        g->handleEvents();
        g->update();
        g->render();
    }

    g->clean();

    return 0;

}
