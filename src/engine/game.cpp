#include "engine/game.h"
#include <iostream>
#include <random>
#include <time.h>
#include <random>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "engine/texturemanager.h"
#include "engine/gamemap.h"
#include "engine/assetmanager.h"
#include "ecs/ecs.h"
#include "ecs/components.h"
#include "helpers/collision.h"
#include "game.h"
#include "scenemanager.h"
#include "scenes/menu.h"
#include "scenes/splash.h"
#include <scenes/gamescene.h>

SDL_Rect Game::camera = {0,0,640,480};

int Game::winWidth = 0;
int Game::winHeigth = 0;
std::set<SDL_Keycode> Game::pressedKeys;
std::set<Uint8> Game::pressedMouseButtons;
std::vector<ColliderComponent*> Game::colliders;
float Game::deltaTime = 0.0f;
float Game::fps = 0.0f;
int Game::frameCount = 0;
uint32_t Game::lastFPSTime = 0;
SceneManager* Game::sceneManager = nullptr;


static SDL_Renderer* staticRenderer = nullptr;

Game::Game(GameLaunchOpts a):
    m_args(a)
{

}


void Game::mainLoop()
{

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    uint32_t frameStart;
    int frameTime;
    static uint32_t lastFrameTime = SDL_GetTicks();
    
    lastFPSTime = SDL_GetTicks();

    if(m_args.skipSplash){
        sceneMgr->pushScene(std::make_unique<GameScene>(*sceneMgr), {true, 0.0});
    }else{
        sceneMgr->pushScene(std::make_unique<SplashScene>(*sceneMgr), {false, 3.0});
    }

    while(running()){
        frameStart = SDL_GetTicks();
        deltaTime = (frameStart - lastFrameTime) / 1000.0f;
        lastFrameTime = frameStart;
        
        frameCount++;
        if (frameStart - lastFPSTime >= 1000) {
            fps = frameCount * 1000.0f / (frameStart - lastFPSTime);
            frameCount = 0;
            lastFPSTime = frameStart;
        }

        handleEvents();
        if(!paused()){
            sceneMgr->updateScenes();
            sceneMgr->renderScenes();
            sceneMgr->processDeferredCommands();
        }
        renderFPS();

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);

    }

    sceneMgr->cleanupScenes();
    clean();
}

void Game::init(const char *title, int xpos, int ypos, int widht, int heigth, bool fullscreen)
{

    Game::camera.w = widht;
    Game::camera.h = heigth;
    srand(static_cast<unsigned int>(time(NULL)));
    int flags = 0;
    if(fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;


    auto sdlFlags =     SDL_INIT_TIMER | \
                        SDL_INIT_AUDIO | \
                        SDL_INIT_VIDEO | \
                        SDL_INIT_EVENTS;
                    /*
                    SDL_INIT_JOYSTICK | \
                    SDL_INIT_GAMECONTROLLER |
                    SDL_INIT_NOPARACHUTE | \
                    */

    if(SDL_Init(sdlFlags) == 0)
    {
        int mixerFlags = MIX_INIT_OGG;
        if(Mix_Init(mixerFlags) != mixerFlags)
        {
            std::cout << "Mix_Init failed: " << Mix_GetError() << std::endl;
            return;
        }

        if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1 )
        {
            std::cout << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
            return;
        }

        Mix_Volume(-1, 32);

       TTF_Init();

        m_win = SDL_CreateWindow(title, xpos, ypos, widht, heigth, flags);
        winHeigth = heigth;
        winWidth = widht;

        if(m_win)
            std::cout << "SDL_CreateWindow OK" << std::endl;

        m_renderer = SDL_CreateRenderer(m_win, -1, 0);
        if(m_renderer){
            SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 0);
            std::cout << "SDL_CreateRenderer OK" << std::endl;
        }

        TextureManager::renderer = m_renderer;
        staticRenderer = m_renderer;

        m_running = true;

        SDL_StopTextInput();

    }else{
        fprintf(stderr, "Failed at SDL init: %s\n", SDL_GetError());
        m_running = false;
    }

    sceneMgr = new SceneManager;

}

void Game::handleEvents()
{
    SDL_Event evt;
    while(SDL_PollEvent(&evt)){
        switch (evt.type) {
        case SDL_QUIT:
            m_running = false;
            break;
        case SDL_KEYUP:
            if(Game::pressedKeys.count(evt.key.keysym.sym))
                Game::pressedKeys.erase(evt.key.keysym.sym);
            break;
        case SDL_KEYDOWN:
            if(evt.key.keysym.sym == SDLK_p)
                togglePause();
            if(evt.key.keysym.sym == SDLK_m)
                dumpStats();
            Game::pressedKeys.insert(evt.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            pressedMouseButtons.insert(evt.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            if(Game::pressedMouseButtons.count(evt.button.button))
                Game::pressedMouseButtons.erase(evt.button.button);
            break;

        default:
            break;
        }
    }

}

void Game::togglePause()
{
    m_paused = !m_paused;
}

void Game::dumpStats()
{
    //std::cout << "Entity count: " << manager.getEntityCount() << std::endl;
}

bool Game::paused()
{
    // TODO andoli: move to gamescene
    return m_paused;
}

void Game::clean()
{
    Mix_Quit();
    SDL_DestroyWindow(m_win);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
    std::cout << "SDL_Quit()" << std::endl;
}


bool Game::running()
{
    return m_running;
}

Vector2D Game::cameraToWorld(Vector2D v)
{
    return {v.x + camera.x, v.y + camera.y};
}

Vector2D Game::worldToCamera(Vector2D v)
{
    return {v.x - camera.x, v.y - camera.y};
}


SDL_Renderer* Game::getRenderer()
{
    return staticRenderer;
}

void Game::renderFPS()
{
    TTF_Font* font = AssetManager::getFont("assets/8bit16.ttf");
    if (!font) return;
    
    SDL_Color green = {0, 255, 0, 255};
    
    // Render FPS
    std::string fpsText = "FPS: " + std::to_string(static_cast<int>(fps));
    SDL_Surface* fpsSurface = TTF_RenderText_Solid(font, fpsText.c_str(), green);
    if (fpsSurface) {
        SDL_Texture* fpsTexture = SDL_CreateTextureFromSurface(staticRenderer, fpsSurface);
        if (fpsTexture) {
            SDL_Rect fpsRect = {10, 10, fpsSurface->w, fpsSurface->h};
            SDL_RenderCopy(staticRenderer, fpsTexture, nullptr, &fpsRect);
            SDL_DestroyTexture(fpsTexture);
        }
        SDL_FreeSurface(fpsSurface);
    }
    
    // Render Entity Count
    int entityCount = 0;
    if (sceneMgr) {
        Scene* currentScene = sceneMgr->getCurrentScene();
        if (currentScene) {
            entityCount = currentScene->getEntityManager().getEntityCount();
        }
    }
    
    std::string entityText = "Entities: " + std::to_string(entityCount);
    SDL_Surface* entitySurface = TTF_RenderText_Solid(font, entityText.c_str(), green);
    if (entitySurface) {
        SDL_Texture* entityTexture = SDL_CreateTextureFromSurface(staticRenderer, entitySurface);
        if (entityTexture) {
            SDL_Rect entityRect = {10, 30, entitySurface->w, entitySurface->h};
            SDL_RenderCopy(staticRenderer, entityTexture, nullptr, &entityRect);
            SDL_DestroyTexture(entityTexture);
        }
        SDL_FreeSurface(entitySurface);
    }
}


