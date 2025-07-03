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
#include "ecs/ecs.h"
#include "ecs/components.h"
#include "helpers/collision.h"
#include "scenes/scenes.h"
#include "game.h"

SDL_Rect Game::camera = {0,0,640,480};

int Game::winWidth = 0;
int Game::winHeigth = 0;
std::set<SDL_Keycode> Game::pressedKeys;
std::set<Uint8> Game::pressedMouseButtons;
std::vector<ColliderComponent*> Game::colliders;

float Game::shakeIntensity = 0.0f;
float Game::shakeDuration = 0.0f;
uint32_t Game::shakeStartTime = 0;
Vector2D Game::shakeOffset = {0.0f, 0.0f};

std::vector<TileData> TileRenderer::tiles;

EntityManager manager;
static SDL_Renderer* staticRenderer = nullptr;


Game::Game()
{

}

Game::~Game()
{

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

    }else{
        fprintf(stderr, "Failed at SDL init: %s\n", SDL_GetError());
        m_running = false;
    }

    stuff(manager);

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
    std::cout << "Entity count: " << manager.getEntityCount() << std::endl;
}

bool Game::paused()
{
    return m_paused;
}

void Game::update()
{
    
    manager.update();
    manager.refresh();

    Vector2D pos = manager.get("player")->getComponent<TransformComponent>().pos;
    camera.x = pos.x - winWidth / 2;
    camera.y = pos.y - winHeigth / 2;

    // Apply boundary constraints first
    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x + camera.w > GameMap::mapWidth ? GameMap::mapWidth - camera.w : camera.x;
    camera.y = camera.y + camera.h > GameMap::mapHeight ? GameMap::mapHeight - camera.h : camera.y;

    applyCameraShake();


}

void inline Game::applyCameraShake()
{
    uint32_t currentTime = SDL_GetTicks();
    if (shakeDuration > 0 && currentTime - shakeStartTime < shakeDuration)
    {
        float progress = (currentTime - shakeStartTime) / shakeDuration;
        float currentIntensity = shakeIntensity * (1.0f - progress);

        float shakeX = (rand() % 200 - 100) / 100.0f * currentIntensity;
        float shakeY = (rand() % 200 - 100) / 100.0f * currentIntensity;

        // Apply shake but keep within bounds
        int newX = camera.x + (int)shakeX;
        int newY = camera.y + (int)shakeY;

        // shake doesn't go outside map boundaries
        if (newX >= 0 && newX + camera.w <= GameMap::mapWidth)
        {
            camera.x = newX;
        }
        if (newY >= 0 && newY + camera.h <= GameMap::mapHeight)
        {
            camera.y = newY;
        }
    }
}
void Game::render()
{
    SDL_RenderClear(m_renderer);

    // Render static tiles first (background)
    TileRenderer::renderTiles(m_renderer, camera);

    for(int g = 0; g != groupLast; g++)
    {
        auto& entities = manager.getGroup(g);
        for(auto i{0}; i < entities.size(); i++)
        {
            entities[i]->draw();
        }
    }

    SDL_RenderPresent(m_renderer);

}


void Game::clean()
{
    Mix_Quit();
    SDL_DestroyWindow(m_win);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
    std::cout << "SDL_Quit()" << std::endl;
}

void Game::addTile(SDL_Texture* sdlTexture, const SDL_Rect& src, const SDL_Rect& dst, SDL_RendererFlip flip)
{
    TileRenderer::addTile(sdlTexture, src, dst, flip);
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

void Game::shakeCamera(float intensity, float duration)
{
    shakeIntensity = intensity;
    shakeDuration = duration;
    shakeStartTime = SDL_GetTicks();
}

SDL_Renderer* Game::getRenderer()
{
    return staticRenderer;
}


void TileRenderer::addTile(SDL_Texture* texture, const SDL_Rect& src, const SDL_Rect& dst, SDL_RendererFlip flip)
{
    tiles.emplace_back(texture, src, dst, flip);
}

void TileRenderer::renderTiles(SDL_Renderer* renderer, const SDL_Rect& camera)
{
    for (const auto& tile : tiles) {
        SDL_Rect cameraDst = tile.dstRect;
        cameraDst.x -= camera.x;
        cameraDst.y -= camera.y;
        
        if (cameraDst.x + cameraDst.w > 0 && cameraDst.x < camera.w &&
            cameraDst.y + cameraDst.h > 0 && cameraDst.y < camera.h) {
            SDL_RenderCopyEx(renderer, tile.texture, &tile.srcRect, &cameraDst, 0, nullptr, tile.flip);
        }
    }
}

void TileRenderer::clearTiles()
{
    tiles.clear();
}


