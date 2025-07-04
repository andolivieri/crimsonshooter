#include "engine/game.h"
#include "helpers/utils.h"

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
            1920, 1080,
            !args.windowed
            );

    g.mainLoop();

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