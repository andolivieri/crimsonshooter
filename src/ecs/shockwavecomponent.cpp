#include "shockwavecomponent.h"
#include "game.h"
#include "texturemanager.h"
#include <algorithm>
#include <cmath>

ShockwaveComponent::ShockwaveComponent(float damageRadius, int numRings, float duration)
    : maxRadius(damageRadius), totalDuration(duration), elapsedTime(0.0f), isComplete(false)
{
    startTime = SDL_GetTicks();
    
    // Create multiple rings with staggered start times and different properties
    rings.reserve(numRings);
    for (int i = 0; i < numRings; ++i) {
        ShockwaveRing ring;
        ring.currentRadius = 0.0f;
        ring.maxRadius = maxRadius + (i * 20.0f); // Each ring slightly larger
        ring.speed = (ring.maxRadius / duration) * (1.0f + i * 0.1f); // Varied speeds
        ring.alpha = 255.0f - (i * 40.0f); // Decreasing opacity
        
        // Color variations - orange to red gradient
        ring.r = 255;
        ring.g = std::max(100, 200 - i * 30);
        ring.b = std::max(0, 100 - i * 50);
        ring.active = true;
        
        rings.push_back(ring);
    }
}

void ShockwaveComponent::init()
{
    transform = &entity->getComponent<TransformComponent>();
}

void ShockwaveComponent::update()
{
    if (!transform || isComplete) return;
    
    uint32_t currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - startTime) / 1000.0f;
    elapsedTime = deltaTime;
    
    updateRings(deltaTime);
    
    // Check if all rings are complete
    bool allComplete = true;
    for (const auto& ring : rings) {
        if (ring.active && ring.currentRadius < ring.maxRadius) {
            allComplete = false;
            break;
        }
    }
    
    if (allComplete || elapsedTime >= totalDuration * 1.5f) {
        isComplete = true;
        entity->destroy();
    }
}

void ShockwaveComponent::draw()
{
    if (!transform || isComplete) return;
    
    float centerX = transform->pos.x + (transform->width / 2.0f) - Game::camera.x;
    float centerY = transform->pos.y + (transform->height / 2.0f) - Game::camera.y;
    
    for (const auto& ring : rings) {
        if (ring.active && ring.currentRadius > 0) {
            drawRing(ring, centerX, centerY);
        }
    }
}

void ShockwaveComponent::updateRings(float deltaTime)
{
    for (auto& ring : rings) {
        if (!ring.active) continue;
        
        // Expand the ring
        ring.currentRadius += ring.speed * deltaTime;
        
        // Fade out as it expands
        float progress = ring.currentRadius / ring.maxRadius;
        if (progress > 0.7f) {
            float fadeProgress = (progress - 0.7f) / 0.3f;
            ring.alpha = 255.0f * (1.0f - fadeProgress);
        }
        
        // Deactivate if fully expanded
        if (ring.currentRadius >= ring.maxRadius) {
            ring.active = false;
        }
    }
}

void ShockwaveComponent::drawRing(const ShockwaveRing& ring, float centerX, float centerY)
{
    if (ring.alpha <= 0) return;
    
    // Set render draw color with alpha
    SDL_SetRenderDrawColor(TextureManager::renderer, ring.r, ring.g, ring.b, 
                          static_cast<Uint8>(std::max(0.0f, ring.alpha)));
    
    // Draw circle using Bresenham's circle algorithm with thickness
    int thickness = 3;
    for (int t = 0; t < thickness; ++t) {
        float radius = ring.currentRadius + t;
        
        // Draw circle outline
        int x = 0;
        int y = static_cast<int>(radius);
        int d = 1 - static_cast<int>(radius);
        
        while (x <= y) {
            // Draw 8 symmetric points
            SDL_RenderDrawPoint(TextureManager::renderer, centerX + x, centerY + y);
            SDL_RenderDrawPoint(TextureManager::renderer, centerX + y, centerY + x);
            SDL_RenderDrawPoint(TextureManager::renderer, centerX - x, centerY + y);
            SDL_RenderDrawPoint(TextureManager::renderer, centerX - y, centerY + x);
            SDL_RenderDrawPoint(TextureManager::renderer, centerX + x, centerY - y);
            SDL_RenderDrawPoint(TextureManager::renderer, centerX + y, centerY - x);
            SDL_RenderDrawPoint(TextureManager::renderer, centerX - x, centerY - y);
            SDL_RenderDrawPoint(TextureManager::renderer, centerX - y, centerY - x);
            
            if (d < 0) {
                d += 2 * x + 3;
            } else {
                d += 2 * (x - y) + 5;
                y--;
            }
            x++;
        }
    }
}