#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "SDL.h"

class TextureManager
{
public:
    static SDL_Texture* loadTexture(const char* filename);
    static void drawTexture(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst, double rotation=0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    static SDL_Renderer* renderer;
};

#endif // TEXTUREMANAGER_H
