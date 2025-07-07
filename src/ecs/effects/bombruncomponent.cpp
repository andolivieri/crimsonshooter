#include "bombruncomponent.h"
#include "engine/game.h"
#include <random>
#include <cmath>


std::pair<Vector2D, Vector2D> getRandomScreenEdgePoints();

BombRunComponent::BombRunComponent(float delaySeconds)
    : m_delayDuration(delaySeconds)
{
}

void BombRunComponent::init()
{

    entity->m_manager.addEntity()
        .addComponent<TimerComponent>(m_delayDuration * ONE_SECOND, false)
        .onTrigger([&](Entity &e)
                   { 
                    spawnBomber(); 
                    e.destroy();
                });
    entity->m_manager.addEntity()
        .addComponent<TimerComponent>(m_delayDuration * ONE_SECOND + 5 * ONE_SECOND, false)
        .onTrigger([&](Entity &e)
                   { 
                    dropBomb(); 
                    e.destroy();
                });
    
}

void BombRunComponent::spawnBomber()
{
    float speed = 5.f;
    Vector2D spriteSize(256, 138);
    int spriteScale = 3;
    auto [startPos, endPos] = getRandomScreenEdgePoints();
    float angle = Math2D::angleBetweenPoints(startPos, endPos);

    auto &bomberEntity = entity->m_manager.addEntity();

    auto &transform = bomberEntity.addComponent<TransformComponent>(
        static_cast<int>(startPos.x),
        static_cast<int>(startPos.y),
        spriteSize.x * spriteScale, spriteSize.y * spriteScale);

    auto &sprite = bomberEntity.addComponent<SpriteComponent>("assets/bomber.png")
                       .setSrcRect({0, 0, (int)spriteSize.x, (int)spriteSize.y})
                       .setAbsolute(false)
                       .setAlpha(120);

    Vector2D velocity = {
        static_cast<float>(std::cos(Math2D::deg2rad(angle))) * speed,
        static_cast<float>(std::sin(Math2D::deg2rad(angle))) * speed
    };

    transform.velocity = velocity;
    transform.rotation = angle + 90;

    bomberEntity.addComponent<DecayComponent>(10 * ONE_SECOND, 10 * ONE_SECOND);
    bomberEntity.addGroup(groupProjectiles);

    m_bomberEntity = &bomberEntity;
}

void BombRunComponent::dropBomb()
{

    if(!m_bombDropped){
        auto enemies = entity->m_manager.getGroup(groupEnemies);
        for (auto enemy : enemies) {
            if (!enemy->hasComponent<FireComponent>() && enemy->hasComponent<DamageModelComponent>()) {
                if(!enemy->getComponent<DamageModelComponent>().isDead()){
                    enemy->addComponent<FireComponent>(10.0f * ONE_SECOND, 5);
                }
            }
        }
        m_bombDropped = true;
    }

    auto &flashEntity = entity->m_manager.addEntity();
    const auto startTimeMsec = SDL_GetTicks();

    flashEntity.addComponent<DumbComponent>().onDraw([startTimeMsec, &flashEntity](){
        auto timediff = SDL_GetTicks() - startTimeMsec;
        float alpha = timediff / (4.f * ONE_SECOND);
        Uint8 alpha2 = static_cast<Uint8>((1.0f - alpha) * 255);
        SDL_SetRenderDrawBlendMode(TextureManager::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(TextureManager::renderer, 255, 255, 255, alpha2);
        SDL_Rect r{0, 0, Game::winWidth, Game::winHeigth};
        SDL_RenderFillRect(TextureManager::renderer, &r);
        SDL_SetRenderDrawBlendMode(TextureManager::renderer, SDL_BLENDMODE_NONE);


        if(alpha >= 1)
            flashEntity.destroy();
    });
    //flashEntity.addComponent<DecayComponent>(2000.f, 2000);
    flashEntity.addGroup(groupOverlayBg);

}


std::pair<Vector2D, Vector2D> getRandomScreenEdgePoints() {

    // returns random opposing pair of point of window

    const float SCREEN_WIDTH = Game::winWidth;
    const float SCREEN_HEIGHT = Game::winHeigth;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> edgeDist(0, 7);
    std::uniform_real_distribution<float> xleftDist(0, SCREEN_WIDTH / 2);
    std::uniform_real_distribution<float> xRightDist(SCREEN_WIDTH / 2, SCREEN_WIDTH);
    std::uniform_real_distribution<float> yTopDist(0, SCREEN_HEIGHT / 2);
    std::uniform_real_distribution<float> yBottomDist(SCREEN_HEIGHT / 2, SCREEN_HEIGHT);

    auto getEdgePoint = [&](int edge) -> Vector2D {
        switch (edge) {
            case 0: return Vector2D(xleftDist(gen), 0.f); // Top-left
            case 1: return Vector2D(xRightDist(gen), 0.f); // Top-right
            case 2: return Vector2D(SCREEN_WIDTH, yTopDist(gen)); // Right-top
            case 3: return Vector2D(SCREEN_WIDTH, yBottomDist(gen)); // Right-bottom
            case 4: return Vector2D(xRightDist(gen), SCREEN_HEIGHT); // Bottom-right
            case 5: return Vector2D(xleftDist(gen), SCREEN_HEIGHT); // Bottom-left
            case 6: return Vector2D(0.f, yBottomDist(gen)); // Left-bottom
            case 7: return Vector2D(0.f, yTopDist(gen)); // Left-bottom
            default: return Vector2D(0, 0);
        }
    };

    int startEdge = edgeDist(gen);
    int endEdge = (startEdge + 4) % 8;

    Vector2D startPos = getEdgePoint(startEdge);
    Vector2D endPos = getEdgePoint(endEdge);

    return {startPos, endPos};
}