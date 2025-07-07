#ifndef BOMBRUNCOMPONENT_H
#define BOMBRUNCOMPONENT_H

#include <SDL.h>
#include "ecs/ecs.h"
#include "ecs/base/transformcomponent.h"
#include "ecs/base/spritecomponent.h"
#include "helpers/vector2d.h"

class BombRunComponent : public Component
{
public:
    BombRunComponent(float delaySeconds = 1.0f);
    
    void init() override;
    
private:
    float m_delayTimer = 0.0f;
    float m_delayDuration = 1.0f;
    bool m_bomberSpawned = false;
    bool m_bombDropped = false;
    
    Entity* m_bomberEntity = nullptr;
    
    void spawnBomber();
    void dropBomb();
};

#endif // BOMBRUNCOMPONENT_H