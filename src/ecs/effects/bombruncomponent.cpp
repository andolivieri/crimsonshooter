#include "bombruncomponent.h"
#include "engine/game.h"
#include <random>
#include <cmath>

BombRunComponent::BombRunComponent(float delaySeconds)
    : m_delayDuration(delaySeconds)
{
}

void BombRunComponent::init()
{
    m_delayTimer = 0.0f;
    m_bomberSpawned = false;
}

void BombRunComponent::update()
{
    if (!m_bomberSpawned) {
        m_delayTimer += Game::deltaTime;
        
        if (m_delayTimer >= m_delayDuration) {
            spawnBomber();
            m_bomberSpawned = true;
        }
    }
    
    // Update bomber movement
    if (m_bomberEntity && m_bomberEntity->hasComponent<TransformComponent>()) {
        auto& transform = m_bomberEntity->getComponent<TransformComponent>();
        
        // Check if bomber has moved off screen and clean up
        if (transform.pos.x < -200 || transform.pos.x > Game::winWidth + 200 ||
            transform.pos.y < -200 || transform.pos.y > Game::winHeigth + 200) {
            
            // Bomber has finished its run, destroy this component's entity
            entity->destroy();
        }
    }
}

void BombRunComponent::spawnBomber()
{
    Vector2D spriteSize(256,138);
    Vector2D direction = generateRandomTrajectory();
    Vector2D startPos = calculateStartPosition(direction);
    Vector2D endPos = calculateEndPosition(direction);
    
    // Create bomber entity
    auto& bomberEntity = entity->m_manager.addEntity("bomber_shadow");
    
    // Add transform at start position
    bomberEntity.addComponent<TransformComponent>(
        static_cast<int>(startPos.x), 
        static_cast<int>(startPos.y), 
        spriteSize.x, spriteSize.y  // Bomber shadow size
    );
    
    // Add sprite with transparency
    auto& sprite = bomberEntity.addComponent<SpriteComponent>("assets/bomber.png").setSrcRect({0,0,(int)spriteSize.x,(int)spriteSize.y});
    sprite.setAbsolute(true);
    sprite.setAlpha(180);  // Semi-transparent (0-255 range)
    
    // Calculate velocity for straight line movement
    float distance = std::sqrt((endPos.x - startPos.x) * (endPos.x - startPos.x) + 
                              (endPos.y - startPos.y) * (endPos.y - startPos.y));
    float flightTime = 3.0f; // 3 seconds to cross the map
    Vector2D velocity = {
        (endPos.x - startPos.x) / flightTime,
        (endPos.y - startPos.y) / flightTime
    };
    
    // Set velocity for movement (we'll update position manually)
    bomberEntity.addComponent<DecayComponent>(flightTime * 1000); // Auto-destroy after flight time
    
    // Store velocity in transform's velocity field if available, or we'll handle movement here
    auto& transform = bomberEntity.getComponent<TransformComponent>();
    transform.velocity = velocity;
    
    // Add to overlay group for rendering on top
    bomberEntity.addGroup(groupProjectiles);
    
    m_bomberEntity = &bomberEntity;
    
    std::cout << "Bomber spawned at (" << startPos.x << ", " << startPos.y 
              << ") heading to (" << endPos.x << ", " << endPos.y << ")" << std::endl;
}

Vector2D BombRunComponent::generateRandomTrajectory()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * M_PI);
    
    float angle = angleDist(gen);
    return {std::cos(angle), std::sin(angle)};
}

Vector2D BombRunComponent::calculateStartPosition(const Vector2D& direction)
{
    // Start position outside the screen in the opposite direction
    float margin = 150.0f;
    Vector2D center = {Game::winWidth / 2.0f, Game::winHeigth / 2.0f};
    
    // Move from center outward in opposite direction
    Vector2D oppositeDir = {-direction.x, -direction.y};
    
    // Find screen boundary intersection
    float screenEdgeDistance = std::max(Game::winWidth, Game::winHeigth) / 2.0f + margin;
    
    return {
        center.x + oppositeDir.x * screenEdgeDistance,
        center.y + oppositeDir.y * screenEdgeDistance
    };
}

Vector2D BombRunComponent::calculateEndPosition(const Vector2D& direction)
{
    // End position outside the screen in the forward direction
    float margin = 150.0f;
    Vector2D center = {Game::winWidth / 2.0f, Game::winHeigth / 2.0f};
    
    // Find screen boundary intersection
    float screenEdgeDistance = std::max(Game::winWidth, Game::winHeigth) / 2.0f + margin;
    
    return {
        center.x + direction.x * screenEdgeDistance,
        center.y + direction.y * screenEdgeDistance
    };
}