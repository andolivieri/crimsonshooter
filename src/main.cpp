#include "engine/game.h"
#include "helpers/utils.h"

static GameLaunchOpts parseArgs(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    GameLaunchOpts args = parseArgs(argc, argv);
    
    Game g(args);

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
        } else if( arg == "-s" || arg == "--skip-splash" ){
            args.skipSplash = true;
        } else if( arg == "-d" || arg == "--debug-colliders" ){
            args.debugColliders = true;
        } else if( arg == "-l" || arg == "--level" ){
            if (i + 1 < argc) {
                args.levelId = argv[++i];
            } else {
                std::cout << "Missing value for " << arg << " (expected a level id, e.g. --level level1)" << std::endl;
            }
        } else {
            std::cout << "Unknown argument: " << arg << std::endl;
        }
    }
    
    return args;
}