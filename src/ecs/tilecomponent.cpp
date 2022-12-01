#include "tilecomponent.h"


TileComponent::TileComponent(){
    memset(&dstRect, 0, sizeof(dstRect));
    memset(&srcRect, 0, sizeof(srcRect));
}

TileComponent::TileComponent(SDL_Texture *s, const SDL_Rect &src, const SDL_Rect &dst, SDL_RendererFlip f)
{
    dstRect = dst;
    srcRect = src;
    sdlTexture = s;
    flip = f;
    position.x = dst.x;
    position.y = dst.y;
}

void TileComponent::init()
{
    entity->addComponent<TransformComponent>(static_cast<float>(dstRect.x),
                                             static_cast<float>(dstRect.y),
                                             dstRect.w,
                                             dstRect.h);
    transform = &entity->getComponent<TransformComponent>();
    entity->addComponent<SpriteComponent>(sdlTexture, srcRect, flip);
    sprite = &entity->getComponent<SpriteComponent>();
}
