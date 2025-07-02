#ifndef STAMINACOMPONENT_H
#define STAMINACOMPONENT_H

#include "ecs/ecs.h"
#include <SDL.h>

class StaminaComponent : public Component
{
public:
    StaminaComponent(float maxStamina = 100.0f);
    
    void init() override;
    void update() override;
    
    bool canSprint() const;
    void consumeStamina(float deltaTime);
    void rechargeStamina(float deltaTime);
    
    float getCurrentStamina() const { return currentStamina; }
    float getMaxStamina() const { return maxStamina; }
    float getStaminaPercentage() const { return currentStamina / maxStamina; }
    
private:
    float maxStamina;
    float currentStamina;
    float consumeRate = 50.0f; // stamina/sec when sprinting
    float rechargeRate = 25.0f; 
    float minSprintStamina = 1.0f; // minimum stamina needed to start sprinting
    
    uint32_t lastUpdateTime;
};

#endif // STAMINACOMPONENT_H