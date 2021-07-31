#include "game.h"
#include "SDL_image.h"
#include <iostream>

SDL_Texture* g_playerTex;
SDL_Rect g_srcRect, g_dstRect;

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

    SDL_Surface* tmp = IMG_Load("assets/player.png");
    g_playerTex = SDL_CreateTextureFromSurface(m_renderer, tmp);
    SDL_FreeSurface(tmp);


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
    case SDL_KEYUP:
        onKeyPress(&evt.key);
        break;
    default:
        break;
    }

}

void Game::update()
{
    cnt++;

    g_dstRect.h = 64;
    g_dstRect.w = 64;
}

void Game::render()
{
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, g_playerTex, NULL, &g_dstRect);
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

    std::cout << "Key pressed" << kc  << std::endl;
    int speed = 10;

    switch (kc) {
    case SDLK_LEFT:
        g_dstRect.x -= speed;
        break;
    case SDLK_RIGHT:
        g_dstRect.x += speed;
        break;

    case SDLK_UP:
        g_dstRect.y -= speed;
        break;

    case SDLK_DOWN:
        g_dstRect.y += speed;
        break;
    default:
        break;
    }
}
