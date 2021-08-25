#include "spritecomponent.h"


void SpriteComponent::_update()
{
    dstRect.x = (int)transform->pos.x;
    dstRect.y = (int)transform->pos.y;
    dstRect.w = transform->width;
    dstRect.h = transform->height;
}
