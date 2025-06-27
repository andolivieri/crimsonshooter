#include "staminacomponent.h"
#include <algorithm>

StaminaComponent::StaminaComponent(float maxStamina)
    : maxStamina(maxStamina), currentStamina(maxStamina)
{
    lastUpdateTime = SDL_GetTicks();
}

void StaminaComponent::init()
{
    lastUpdateTime = SDL_GetTicks();
}

void StaminaComponent::update()
{
    
}

bool StaminaComponent::canSprint() const
{
    return currentStamina >= minSprintStamina;
}

void StaminaComponent::consumeStamina(float deltaTime)
{
    currentStamina -= consumeRate * deltaTime;
    currentStamina = std::max(0.0f, currentStamina);
}

void StaminaComponent::rechargeStamina(float deltaTime)
{
    currentStamina += rechargeRate * deltaTime;
    currentStamina = std::min(maxStamina, currentStamina);
}