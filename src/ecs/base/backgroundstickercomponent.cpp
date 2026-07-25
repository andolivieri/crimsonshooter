#include "backgroundstickercomponent.h"
#include "spritecomponent.h"
#include "helpers/stickerbaker.h"
#include <SDL.h>

BackgroundStickerComponent::BackgroundStickerComponent(int group)
    : m_group(group)
{
}

void BackgroundStickerComponent::unmount()
{
    if(entity->hasComponent<SpriteComponent>()){
        auto& sprite = entity->getComponent<SpriteComponent>();
        StickerBaker::enqueue(sprite.snapshot(m_group));
    }
}
