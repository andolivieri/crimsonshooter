#include "grenadecomponent.h"
#include "game.h"
#include "grenadeprojectile.h"
#include "spritecomponent.h"
#include "texturemanager.h"
#include <SDL.h>
#include <algorithm>
#include <cmath>

void GrenadeComponent::init()
{
    input = &entity->getComponent<InputComponent>();
    transform = &entity->emplaceComponent<TransformComponent>();
}

void GrenadeComponent::update()
{
    if (!input || !transform) return;
    
    // Check for grenade button press/release
    for (auto& event : input->frameEvents) {
        if (event.button == BTN_GRENADE) {
            if (event.evt == BTN_PRESS && !isCharging) {
                // Start charging
                isCharging = true;
                chargeLevel = 0.0f;
                chargeStartTime = SDL_GetTicks();
            }
            else if (event.evt == BTN_RELEASE && isCharging) {
                // Release grenade
                throwGrenade(chargeLevel);
                isCharging = false;
                chargeLevel = 0.0f;
            }
        }
    }
    
    // Update charge level while charging
    if (isCharging) {
        uint32_t currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - chargeStartTime) / 1000.0f; // Convert to seconds
        chargeLevel = std::min(deltaTime * chargeSpeed, maxChargeLevel);
    }
}

void GrenadeComponent::draw()
{
    if (isCharging) {
        drawChargeBar();
    }
}

void GrenadeComponent::throwGrenade(float chargeLevel)
{
    if (!transform) return;
    
    // Calculate throw distance based on charge level
    float throwDistance = 100.0f + (chargeLevel * 300.0f); // 100-400 pixel range
    
    // Get mouse position for direction
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    // Convert screen coordinates to world coordinates
    Vector2D playerPos = transform->pos;
    Vector2D targetPos(mouseX + Game::camera.x, mouseY + Game::camera.y);
    
    // Calculate direction vector
    Vector2D direction;
    direction.x = targetPos.x - playerPos.x;
    direction.y = targetPos.y - playerPos.y;
    
    // Normalize direction vector
    float length = direction.magnitude();
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    
    // Calculate final target position based on charge level
    Vector2D finalTarget;
    finalTarget.x = playerPos.x + (direction.x * throwDistance);
    finalTarget.y = playerPos.y + (direction.y * throwDistance);
    
    // Create grenade projectile
    auto& grenade = entity->m_manager.addEntity();
    grenade.addComponent<TransformComponent>(playerPos.x, playerPos.y, 32, 32);
    grenade.addComponent<SpriteComponent>("assets/bomb.png").setSrcRect({0, 0, 128, 128});
    grenade.addComponent<GrenadeProjectileComponent>(playerPos, finalTarget);
    grenade.addGroup(groupProjectiles);
}

void GrenadeComponent::drawChargeBar()
{
    if (!transform) return;
    
    // Draw charge bar next to player
    Vector2D barPos = transform->pos;
    barPos.x += transform->width + 10; // 10 pixels to the right of player
    barPos.y += transform->height / 2 - 25; // Center vertically
    
    // Apply camera offset (convert world to screen coordinates)
    barPos.x -= Game::camera.x;
    barPos.y -= Game::camera.y;
    
    // Bar dimensions
    int barWidth = 8;
    int barHeight = 50;
    int fillHeight = static_cast<int>(barHeight * chargeLevel);
    
    // Draw background bar (empty)
    SDL_Rect bgRect = {
        static_cast<int>(barPos.x),
        static_cast<int>(barPos.y),
        barWidth,
        barHeight
    };
    SDL_SetRenderDrawColor(TextureManager::renderer, 50, 50, 50, 255); // Dark gray
    SDL_RenderFillRect(TextureManager::renderer, &bgRect);
    
    // Draw filled portion (charge level)
    if (fillHeight > 0) {
        SDL_Rect fillRect = {
            static_cast<int>(barPos.x),
            static_cast<int>(barPos.y + barHeight - fillHeight),
            barWidth,
            fillHeight
        };
        
        // Color changes based on charge level
        if (chargeLevel < 0.5f) {
            SDL_SetRenderDrawColor(TextureManager::renderer, 255, 255, 0, 255); // Yellow
        } else if (chargeLevel < 0.8f) {
            SDL_SetRenderDrawColor(TextureManager::renderer, 255, 165, 0, 255); // Orange
        } else {
            SDL_SetRenderDrawColor(TextureManager::renderer, 255, 0, 0, 255); // Red
        }
        
        SDL_RenderFillRect(TextureManager::renderer, &fillRect);
    }
    
    // Draw border
    SDL_SetRenderDrawColor(TextureManager::renderer, 255, 255, 255, 255); // White border
    SDL_RenderDrawRect(TextureManager::renderer, &bgRect);
}