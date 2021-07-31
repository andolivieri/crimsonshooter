#include "game.h"
#include <iostream>

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
            SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
            std::cout << "SDL_CreateRenderer OK" << std::endl;
        }


        m_running = true;

    }else{
        m_running = false;
    }
}

void Game::handleEvents()
{
    SDL_Event evt;
    SDL_PollEvent(&evt);
    switch (evt.type) {
    case SDL_QUIT:
        m_running = false;
        break;
    default:
        break;
    }

}

void Game::update()
{

}

void Game::render()
{
    SDL_RenderClear(m_renderer);
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
