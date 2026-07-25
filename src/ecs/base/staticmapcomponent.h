#ifndef STATICMAPCOMPONENT_H
#define STATICMAPCOMPONENT_H

#include "ecs/ecs.h"
#include <SDL.h>

// Draws a whole pre-baked map layer (single texture for all static tiles)
class StaticMapComponent : public Component
{
public:
    explicit StaticMapComponent(SDL_Texture* texture);

    void draw() override;

private:
    SDL_Texture* m_texture;
};

#endif // STATICMAPCOMPONENT_H
