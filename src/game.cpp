#include "game.h"
#include <iostream>
#include "SDL_image.h"
#include "texturemanager.h"
#include "gamemap.h"
#include "ecs.h"
#include "ecs/components.h"
#include "collision.h"

GameMap* g_map = nullptr;

std::set<SDL_Keycode> Game::pressedKeys;
std::vector<ColliderComponent*> Game::colliders;

EntityManager manager;
auto& newEnemy(manager.addEntity());
auto& wall(manager.addEntity());
auto& newPlayer(manager.addEntity());

enum groupLabels
{
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders,
    groupLast
};

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

        TextureManager::renderer = m_renderer;


        m_running = true;

    }else{
        m_running = false;
    }

    g_map = new GameMap();

    GameMap::LoadMap("assets/themap.json");

    newPlayer.addComponent<TransformComponent>(100.f,100.f, 64,64);
    newPlayer.getComponent<TransformComponent>().speed = 1.5;
    newPlayer.getComponent<TransformComponent>().width = 64;
    newPlayer.getComponent<TransformComponent>().height = 64;
    newPlayer.addComponent<SpriteComponent>("assets/player.png")
            .setSrcRect({0,0,16,16})
            .addAnimation("idle", {0, 2, 600 })
            .addAnimation("moving", {1, 4, 200 });
    newPlayer.addComponent<InputComponent>();
    newPlayer.addComponent<ColliderComponent>("player");
    newPlayer.addGroup(groupPlayers);

/*
    newEnemy.addComponent<TransformComponent>(0.f,0.f, 128,128);
    newEnemy.addComponent<AIComponent>(newPlayer);
    newEnemy.addComponent<SpriteComponent>("assets/foe.png");
*/

    wall.addComponent<ColliderComponent>();
    wall.addComponent<TransformComponent>(155, 250, 32, 512);
    wall.addComponent<SpriteComponent>("assets/wall.png");
    wall.addComponent<ColliderComponent>("wall");
    wall.addGroup(groupMap);

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
            Game::pressedKeys.insert(evt.key.keysym.sym);
            break;
        default:
            break;
        }
    }


    // TODO dispatch eventi? Intanto filtro solo key up/down

    //event = evt;

}

void Game::update()
{
    manager.update();

}

void Game::render()
{
    SDL_RenderClear(m_renderer);

    for(int g = 0; g != groupLast; g++)
    {
        auto& entities = manager.getGroup(g);
        for(auto e : entities) e->draw();
    }

    /*
    for(auto& c : colliders)
        if(c != &newPlayer.getComponent<ColliderComponent>() &&
                Collision::AABB(*c, newPlayer.getComponent<ColliderComponent>())){
            std::cout << "Player hit: " << c->tag << std::endl;
        }
*/
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

