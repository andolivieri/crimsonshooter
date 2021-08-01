#include "game.h"
#include <iostream>
#include "SDL_image.h"
#include "texturemanager.h"
#include "gameobject.h"
#include "gamemap.h"
#include "ecs.h"
#include "Components.h"

GameObject* g_player = nullptr;
GameObject* g_enemy = nullptr;
GameMap* g_map = nullptr;

EntityManager manager;
auto& newPlayer(manager.addEntity());

Game::Game()
{

}

Game::~Game()
{

}

void Game::init(const char *title, int xpos, int ypos, int widht, int heigth, bool fullscreen)
{

    int flags = 0;
    if(fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "SDL_Init OK" << std::endl;

        m_win = SDL_CreateWindow(title, xpos, ypos, widht, heigth, flags);
        if(m_win)
            std::cout << "SDL_CreateWindow OK" << std::endl;

        m_renderer = SDL_CreateRenderer(m_win, -1, 0);
        if(m_renderer){
            SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 0);
            std::cout << "SDL_CreateRenderer OK" << std::endl;
        }


        m_running = true;

    }else{
        m_running = false;
    }

    g_player = new GameObject("assets/player.png", m_renderer);
    g_enemy = new GameObject("assets/enemy.png", m_renderer);
    g_map = new GameMap(m_renderer);

    g_player->x = 300;
    g_player->y = 300;
    g_enemy->x = 80;
    g_enemy->y = 80;


    newPlayer.addComponent<PositionComponent>();

}

void Game::handleEvents()
{
    SDL_Event evt;
    SDL_PollEvent(&evt);
    switch (evt.type) {
    case SDL_QUIT:
        m_running = false;
        break;
    case SDL_KEYDOWN:
        onKeyPress(&evt.key);
        break;
    default:
        break;
    }

}

void Game::update()
{
    g_player->update();

    if(g_enemy->x > g_player->x)
        g_enemy->x--;
    else
        g_enemy->x++;

    if(g_enemy->y > g_player->y)
        g_enemy->y--;
    else
        g_enemy->y++;


    g_enemy->update();
    manager.update();
}

void Game::render()
{
    SDL_RenderClear(m_renderer);
    g_map->DrawMap();
    g_player->render();
    g_enemy->render();
    manager.draw();
    SDL_RenderPresent(m_renderer);

}

void Game::clean()
{
    SDL_DestroyWindow(m_win);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
    std::cout << "SDL_Quit()" << std::endl;
}

bool Game::running()
{
    return m_running;
}

void Game::onKeyPress(SDL_KeyboardEvent *key)
{
    SDL_Keycode kc = key->keysym.sym;

    std::cout << "Key pressed: " << SDL_GetKeyName(kc)  << std::endl;
    int speed = 10;

    switch (kc) {
    case SDLK_LEFT:
    case SDLK_a:
        g_player->x -= speed;
        break;
    case SDLK_RIGHT:
    case SDLK_d:
        g_player->x += speed;
        break;

    case SDLK_UP:
    case SDLK_w:
        g_player->y -= speed;
        break;

    case SDLK_DOWN:
    case SDLK_s:
        g_player->y += speed;
        break;
    default:
        break;
    }
}
