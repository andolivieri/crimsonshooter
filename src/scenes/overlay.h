#ifndef OVERLAY_H
#define OVERLAY_H

#include "ecs.h"
#include "ecs/components.h"

void overlay(EntityManager& manager)
{
    auto& theOverlay(manager.addEntity("overlay"));
    thePlayer.getComponent<TransformComponent>().pos.x = Game::winWidth / 2.f;


}

#endif // OVERLAY_H
