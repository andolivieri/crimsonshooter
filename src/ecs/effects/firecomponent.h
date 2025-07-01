#ifndef FIRECOMPONENT_H
#define FIRECOMPONENT_H

#include <SDL.h>
#include "ecs.h"
#include "ecs/core/transformcomponent.h"
#include "ecs/core/relationshipcomponent.h"
#include "utils.h"

class FireComponent : public Component {
public:
    FireComponent(float duration = 10.0f * TIME_SECOND, int damagePerSecond = 5);
    
    void init() override;
    void update() override;
    
    bool isOnFire() const { return active; }
    
private:
    float duration;
    int damagePerSecond;
    uint32_t startTime;
    uint32_t lastDamageTime;
    bool active = true;
    
    TransformComponent* target_transform = nullptr;
    
    void createFireVisual();
    void applyFireDamage();
};

#endif // FIRECOMPONENT_H