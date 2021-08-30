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

GameMap* g_map = nullptr;

std::set<SDL_Keycode> Game::pressedKeys;
std::vector<ColliderComponent*> Game::colliders;

EntityManager manager;
auto& newEnemy(manager.addEntity());
auto& wall(manager.addEntity());
auto& newPlayer(manager.addEntity());


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

    g_map = new GameMap();

    GameMap::LoadMap("assets/themap.json");

    newPlayer.addComponent<TransformComponent>(100.f,100.f, 64,64);
    newPlayer.getComponent<TransformComponent>().speed = 2;
    newPlayer.getComponent<TransformComponent>().width = 64;
    newPlayer.getComponent<TransformComponent>().height = 64;
    newPlayer.getComponent<TransformComponent>().pos.x = widht / 2.f;
    newPlayer.getComponent<TransformComponent>().pos.y = heigth / 2.f;
    newPlayer.addComponent<SpriteComponent>("assets/player.png")
            .setSrcRect({0,0,16,16})
            .addAnimation("idle", {0, 2, 600 })
            .addAnimation("fast", {1, 4, 100 })
            .addAnimation("moving", {1, 4, 200 });
    newPlayer.addComponent<InputComponent>();
    newPlayer.addComponent<ColliderComponent>("player");
    newPlayer.addGroup(groupPlayers);

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
        case SDL_MOUSEBUTTONDOWN:

            SDL_Point mousePt;
            SDL_GetMouseState(&mousePt.x,&mousePt.y);
            {
            auto& e = manager.addEntity();
            e.addComponent<ProjectileComponent>(
                newPlayer.getComponent<TransformComponent>().center(),
                Vector2D{mousePt.x, mousePt.y});
            e.addComponent<SpriteComponent>("assets/projectile.png")
                    .setSrcRect({2,2,4,4});
            std::cout << "Shoot" << std::endl;
            }
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

    auto& projectiles = manager.getGroup(groupProjectiles);
    auto& enemies = manager.getGroup(groupEnemies);

    for(auto bullet : projectiles)
    {
        for(auto enemy : enemies){

            DamageModelComponent& enemyDamage = enemy->getComponent<DamageModelComponent>();
            ColliderComponent& enemyCC = enemy->getComponent<ColliderComponent>();

            ProjectileComponent& pc = bullet->getComponent<ProjectileComponent>();
            ColliderComponent& cc = bullet->getComponent<ColliderComponent>();

            if(Collision::AABB(cc, enemyCC))
            {
                enemyDamage.health -= pc.damage;
                std::cout << "Tag " << enemyCC.tag << " Enemy hit. Damage=" << pc.damage << " Enemy health: " << enemyDamage.health << std::endl;
                bullet->setActive(false);
            }

        }

    }

    auto howManyEnemies = enemies.size();
    while(howManyEnemies++ < 10)
    {
        spawnFoe();
    }


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

void Game::spawnFoe()
{

    static long enemyCount = 0;

    auto& theFoe = manager.addEntity();
    theFoe.addComponent<TransformComponent>(0.f,0.f, 64,64);
    theFoe.addComponent<SpriteComponent>("assets/foe.png")
            .setSrcRect({0,0,32,32})
            .addAnimation("idle", {0, 1, 100 })
            .addAnimation("dying", {1, 4, 300 })
            .addAnimation("moving", {0, 4, 200 });
    theFoe.addComponent<DamageModelComponent>(30);
    theFoe.addComponent<ColliderComponent>("foe" + std::to_string(enemyCount++));
    theFoe.addComponent<AIComponent>(newPlayer);
    theFoe.addGroup(groupEnemies);

    Vector2D spawnPt;

    switch (rand()% 4) {
    case 0:
        // CENTER
        spawnPt.y = -128;
        spawnPt.x = rand() % winWidth;
        break;
    case 1:
        // LEFT
        spawnPt.y = rand() % winHeigth;
        spawnPt.x = 0;
        break;
    case 2:
        // RIGHT
        spawnPt.y = rand() % winHeigth;
        spawnPt.x = winWidth;
        break;
    case 3:
        // BOTTOM
        spawnPt.y = winHeigth;
        spawnPt.x = rand() % winWidth;
        break;
    default:
        break;
    }


    theFoe.getComponent<TransformComponent>().pos = spawnPt;
    float speed = .1f + (rand() / (float)RAND_MAX );
    theFoe.getComponent<AIComponent>().speed = speed;



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

