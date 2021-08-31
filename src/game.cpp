#include "game.h"
#include <iostream>
#include <random>
#include <time.h>
#include <random>
#include "SDL_image.h"
#include "texturemanager.h"
#include "gamemap.h"
#include "ecs.h"
#include "ecs/components.h"
#include "collision.h"
#include "scenes/scenes.h"


int Game::winWidth = 0;
int Game::winHeigth = 0;
std::set<SDL_Keycode> Game::pressedKeys;
std::set<Uint8> Game::pressedMouseButtons;
std::vector<ColliderComponent*> Game::colliders;

EntityManager manager;


Game::Game()
{

}

Game::~Game()
{

}

void Game::init(const char *title, int xpos, int ypos, int widht, int heigth, bool fullscreen)
{

    srand(time(NULL));
    int flags = 0;
    if(fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "SDL_Init OK" << std::endl;

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


        m_running = true;

    }else{
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

bool Game::paused()
{
    return m_paused;
}

void Game::update()
{



    manager.update();
    manager.refresh();

}

void Game::render()
{
    SDL_RenderClear(m_renderer);

    for(int g = 0; g != groupLast; g++)
    {
        auto& entities = manager.getGroup(g);
        for(auto e : entities) e->draw();
    }

    SDL_RenderPresent(m_renderer);

}


void Game::clean()
{
    SDL_DestroyWindow(m_win);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
    std::cout << "SDL_Quit()" << std::endl;
}

void Game::addTile(SDL_Texture* sdlTexture, const SDL_Rect& src, const SDL_Rect& dst, SDL_RendererFlip flip)
{
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(sdlTexture, src, dst, flip);
    tile.addGroup(groupMap);
}

bool Game::running()
{
    return m_running;
}

