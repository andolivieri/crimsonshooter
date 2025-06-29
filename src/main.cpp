#include "Game.h"
#include "utils.h"

struct GameLaunchOpts {
    bool windowed = false; // -w --windowed
    
    GameLaunchOpts() = default;
};
static GameLaunchOpts parseArgs(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    GameLaunchOpts args = parseArgs(argc, argv);
    
    Game g;

    g.init("Crimson Shooter",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1280, 720,
            !args.windowed
            );

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    uint32_t frameStart;
    int frameTime;

    while(g.running()){
        frameStart = SDL_GetTicks();

        g.handleEvents();
        if(!g.paused()){
            g.update();
            g.render();
        }

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);

    }

    g.clean();

    return 0;

}

static GameLaunchOpts parseArgs(int argc, char* argv[])
{
    GameLaunchOpts args;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-w" || arg == "--windowed") {
            args.windowed = true;
        } else {
            std::cout << "Unknown argument: " << arg << std::endl;
        }
    }
    
    return args;
}