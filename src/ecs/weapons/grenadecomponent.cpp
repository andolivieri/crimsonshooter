#include "grenadecomponent.h"
#include "game.h"
#include "grenadeprojectile.h"
#include "ecs/core/spritecomponent.h"
#include "texturemanager.h"
#include "math2d.h"
#include <SDL.h>
#include <algorithm>
#include <cmath>

void GrenadeComponent::init()
{
    input = &entity->getComponent<InputComponent>();
    transform = &entity->getComponent<TransformComponent>();
}

void GrenadeComponent::update()
{
    if (!input || !transform) return;

    auto wBay = entity->m_manager.get("player")->getComponent<WeaponBayComponent>();
    
    // Check for grenade button press/release
    for (auto& event : input->frameEvents) {
        if (event.button == BTN_GRENADE) {
            if (event.evt == BTN_PRESS && !isCharging && wBay.grenadeCount > 0) {
                // Start charging
                isCharging = true;
                wBay.grenadeCount = wBay.grenadeCount - 1;
                chargeLevel = 0.0f;
                chargeStartTime = SDL_GetTicks();
                auto& click = entity->m_manager.addEntity();
                click.addComponent<SoundComponent>().play("assets/sounds/grenade_load.ogg", 0, 4);
                click.addComponent<DecayComponent>(2000);
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
        drawTrajectoryLine();
    }
}

void GrenadeComponent::throwGrenade(float chargeLevel)
{
    if (!transform) return;
    
    // Calculate throw distance based on charge level
    float throwDistance = 10.0f + (chargeLevel * 300.0f); // 100-400 pixel range
    
    // Get mouse position for direction
    SDL_Point mousePt;
    SDL_GetMouseState(&mousePt.x, &mousePt.y);
    
    // Convert screen coordinates to world coordinates using the proper camera function
    Vector2D playerPos = transform->center();
    Vector2D targetPos = calculateLandingPoint(chargeLevel);

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
    grenade.addComponent<SpriteComponent>("assets/grenade.png").setSrcRect({0, 0, 32, 32});
    grenade.addComponent<GrenadeProjectileComponent>(playerPos, targetPos);
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

Vector2D GrenadeComponent::calculateLandingPoint(float chargeLevel)
{
    if (!transform) return Vector2D(0, 0);
    
    // Calculate throw distance based on charge level (same formula as throwGrenade)
    float throwDistance = 10.0f + (chargeLevel * 300.0f);
    
    // Get mouse position for direction
    SDL_Point mousePt;
    SDL_GetMouseState(&mousePt.x, &mousePt.y);
    
    // Convert screen coordinates to world coordinates
    Vector2D playerPos = transform->center();
    Vector2D targetPos = Game::cameraToWorld(Vector2D(mousePt.x, mousePt.y));
    
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
    
    // Calculate final landing position based on charge level
    Vector2D landingPoint;
    landingPoint.x = playerPos.x + (direction.x * throwDistance);
    landingPoint.y = playerPos.y + (direction.y * throwDistance);
    
    return landingPoint;
}

void GrenadeComponent::drawTrajectoryLine()
{
    if (!transform) return;
    
    Vector2D playerPos = transform->center();
    Vector2D landingPoint = calculateLandingPoint(chargeLevel);
    
    // Convert world coordinates to screen coordinates
    Vector2D playerScreenPos = Game::worldToCamera(playerPos);
    Vector2D landingScreenPos = Game::worldToCamera(landingPoint);
    
    // Set line color (green for low charge, yellow for medium, red for high)
    if (chargeLevel < 0.3f) {
        SDL_SetRenderDrawColor(TextureManager::renderer, 0, 255, 0, 255); // Green
    } else if (chargeLevel < 0.7f) {
        SDL_SetRenderDrawColor(TextureManager::renderer, 255, 255, 0, 255); // Yellow
    } else {
        SDL_SetRenderDrawColor(TextureManager::renderer, 255, 0, 0, 255); // Red
    }
    
    // Draw dotted line
    float totalDistance = Math2D::distanceBetweenPoints(playerScreenPos, landingScreenPos);
    float dotSpacing = 8.0f; // pixels between dots
    int numDots = static_cast<int>(totalDistance / dotSpacing);
    
    if (numDots > 0) {
        for (int i = 0; i <= numDots; ++i) {
            float t = static_cast<float>(i) / numDots;
            
            int dotX = static_cast<int>(playerScreenPos.x + t * (landingScreenPos.x - playerScreenPos.x));
            int dotY = static_cast<int>(playerScreenPos.y + t * (landingScreenPos.y - playerScreenPos.y));
            
            // Draw small circle for each dot
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx*dx + dy*dy <= 1) { // Circle approximation
                        SDL_RenderDrawPoint(TextureManager::renderer, dotX + dx, dotY + dy);
                    }
                }
            }
        }
    }
    
    // Draw landing point indicator (larger circle)
    int landingX = static_cast<int>(landingScreenPos.x);
    int landingY = static_cast<int>(landingScreenPos.y);
    
    // Draw circle using Bresenham's algorithm
    int radius = 5;
    int x = 0;
    int y = radius;
    int d = 1 - radius;
    
    while (x <= y) {
        // Draw 8 symmetric points
        SDL_RenderDrawPoint(TextureManager::renderer, landingX + x, landingY + y);
        SDL_RenderDrawPoint(TextureManager::renderer, landingX + y, landingY + x);
        SDL_RenderDrawPoint(TextureManager::renderer, landingX - x, landingY + y);
        SDL_RenderDrawPoint(TextureManager::renderer, landingX - y, landingY + x);
        SDL_RenderDrawPoint(TextureManager::renderer, landingX + x, landingY - y);
        SDL_RenderDrawPoint(TextureManager::renderer, landingX + y, landingY - x);
        SDL_RenderDrawPoint(TextureManager::renderer, landingX - x, landingY - y);
        SDL_RenderDrawPoint(TextureManager::renderer, landingX - y, landingY - x);
        
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}