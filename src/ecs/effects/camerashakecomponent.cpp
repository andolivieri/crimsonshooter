#include "camerashakecomponent.h"
#include "engine/game.h"
#include "engine/gamemap.h"
#include <cstdlib>
#include <iostream>

CameraShakeComponent::CameraShakeComponent(float intensity, float duration)
    : shakeIntensity(intensity), shakeDuration(duration), active(true)
{
    shakeStartTime = SDL_GetTicks();
}

void CameraShakeComponent::init()
{
    
}

void CameraShakeComponent::update()
{
    if (!active) return;
    
    uint32_t currentTime = SDL_GetTicks();
    if (shakeDuration > 0 && currentTime - shakeStartTime < shakeDuration)
    {
        float progress = (currentTime - shakeStartTime) / shakeDuration;
        float currentIntensity = shakeIntensity * (1.0f - progress);

        float shakeX = (rand() % 200 - 100) / 100.0f * currentIntensity;
        float shakeY = (rand() % 200 - 100) / 100.0f * currentIntensity;

        // shake within bounds
        int newX = Game::camera.x + (int)shakeX;
        int newY = Game::camera.y + (int)shakeY;

        // shake doesn't go outside map boundaries
        if (newX >= 0 && newX + Game::camera.w <= GameMap::mapWidth)
        {
            Game::camera.x = newX;
        }
        if (newY >= 0 && newY + Game::camera.h <= GameMap::mapHeight)
        {
            Game::camera.y = newY;
        }
    }
    
}