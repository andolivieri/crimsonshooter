#ifndef FIRECOMPONENT_H
#define FIRECOMPONENT_H

#include <SDL.h>
#include "ecs/ecs.h"
#include "ecs/base/transformcomponent.h"
#include "ecs/base/relationshipcomponent.h"
#include "ecs/base/spritecomponent.h"
#include "ecs/gameplay/damagemodel.h"
#include "helpers/utils.h"

class FireComponent : public Component {
public:
    FireComponent(float duration = 10.0f * ONE_SECOND, int damagePerSecond = 5);
    
    void init() override;
    void update() override;
    
    bool isOnFire() const { return active; }
    
private:
    float duration;
    int damagePerSecond;
    uint32_t startTime;
    uint32_t lastDamageTime;
    bool active = true;
    
    TransformComponent* transform = nullptr;
    DamageModelComponent* damageModel = nullptr;
    SpriteComponent* sprite = nullptr;
    RelationshipComponent* relationship = nullptr;
    
    void createFireVisual();
    void applyFireDamage();
};

#endif // FIRECOMPONENT_H