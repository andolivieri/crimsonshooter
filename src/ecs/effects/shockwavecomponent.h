#ifndef SHOCKWAVECOMPONENT_H
#define SHOCKWAVECOMPONENT_H

#include "ecs/ecs.h"
#include "ecs/base/transformcomponent.h"
#include <vector>
#include <SDL.h>

struct ShockwaveRing {
    float currentRadius;
    float maxRadius;
    float speed;
    float alpha;
    Uint8 r, g, b;
    bool active;
};

class ShockwaveComponent : public Component
{
public:
    ShockwaveComponent(float damageRadius = 100.0f, int numRings = 3, float duration = 1.0f);
    
    void init() override;
    void update() override;
    void draw() override;
    
private:
    TransformComponent* transform = nullptr;
    
    std::vector<ShockwaveRing> rings;
    float maxRadius;
    float totalDuration;
    float elapsedTime;
    uint32_t startTime;
    bool isComplete;
    
    void updateRings(float deltaTime);
    void drawRing(const ShockwaveRing& ring, float centerX, float centerY);
};

#endif // SHOCKWAVECOMPONENT_H