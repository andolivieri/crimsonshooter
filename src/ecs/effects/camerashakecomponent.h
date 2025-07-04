#ifndef CAMERASHAKECOMPONENT_H
#define CAMERASHAKECOMPONENT_H

#include "ecs/ecs.h"
#include "helpers/vector2d.h"
#include <SDL.h>

class CameraShakeComponent : public Component
{
public:
    CameraShakeComponent(float intensity, float duration);
    
    void init() override;
    void update() override;
    
    bool isActive() const { return active; }
    
private:
    float shakeIntensity;
    float shakeDuration;
    uint32_t shakeStartTime;
    bool active;
};

#endif // CAMERASHAKECOMPONENT_H