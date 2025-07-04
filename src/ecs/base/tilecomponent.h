#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include "ecs/ecs.h"
#include "transformcomponent.h"
#include <SDL.h>

class TileComponent : public Component
{
public:
    TileComponent(SDL_Texture* texture, const SDL_Rect& srcRect, SDL_RendererFlip flip = SDL_FLIP_NONE);
    
    void init() override;
    void update() override;
    void draw() override;
    
private:
    SDL_Texture* m_texture;
    SDL_Rect m_srcRect;
    SDL_RendererFlip m_flip;
    TransformComponent* m_transform;
};

#endif // TILECOMPONENT_H