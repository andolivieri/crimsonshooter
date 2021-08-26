#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <string>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"
#include "spritecomponent.h"
#include "../game.h"


class TileComponent : public Component
{
public:
    TransformComponent* transform;
    SpriteComponent* sprite;

    SDL_Rect dstRect;
    SDL_Rect srcRect;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    SDL_Texture* sdlTexture;

    TileComponent(){
        memset(&dstRect, 0, sizeof(dstRect));
        memset(&srcRect, 0, sizeof(srcRect));
    }

    TileComponent(SDL_Texture* s, const SDL_Rect& src, const SDL_Rect& dst, SDL_RendererFlip f)
    {
        dstRect = dst;
        srcRect = src;
        sdlTexture = s;
        flip = f;
    }

    void init() override
    {
        entity->addComponent<TransformComponent>(dstRect.x,
                                                 dstRect.y,
                                                 dstRect.w,
                                                 dstRect.h);
        transform = &entity->getComponent<TransformComponent>();
        entity->addComponent<SpriteComponent>(sdlTexture, srcRect, flip);
        sprite = &entity->getComponent<SpriteComponent>();
    }
};

#endif // TILECOMPONENT_H
