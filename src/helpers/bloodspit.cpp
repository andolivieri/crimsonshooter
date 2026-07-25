#include "helpers/bloodspit.h"
#include "ecs/ecs.h"
#include "ecs/base/transformcomponent.h"
#include "ecs/base/spritecomponent.h"
#include "ecs/base/backgroundstickercomponent.h"
#include "engine/game.h"
#include <cmath>


void createBloodSpit(Entity &target)
{
    auto &bloodSpit = target.m_manager.addEntity();
    TransformComponent targetTt = target.getComponent<TransformComponent>();
    targetTt.velocity.x = targetTt.velocity.y = 0;

    targetTt.rotation = rand() % 180;
    targetTt.pos.x += std::pow(-1, rand() % 3) * (rand() % targetTt.width / 2);
    targetTt.pos.y += std::pow(-1, rand() % 3) * (rand() % targetTt.height / 2);
    bloodSpit.addComponent<TransformComponent>(targetTt);
    const auto bf = 10;
    bloodSpit.addComponent<SpriteComponent>("assets/blood.png")
        .addAnimation("splat", {0, 0, 3 + (rand() % (bf - 4)), 20})
        .setOnAnimationEnd([&bloodSpit]{ 
            bloodSpit.destroy(); 
        })
        //.showFrame(true)
        .play("splat", 1);
    bloodSpit.addGroup(groupBloodPatches);
    bloodSpit.addComponent<BackgroundStickerComponent>(groupBloodPatches);
}
