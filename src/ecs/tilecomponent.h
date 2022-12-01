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

    TileComponent();

    TileComponent(SDL_Texture* s, const SDL_Rect& src, const SDL_Rect& dst, SDL_RendererFlip f);

    void init() override;
    Vector2D position;
};

#endif // TILECOMPONENT_H
