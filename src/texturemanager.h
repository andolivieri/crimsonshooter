#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "SDL.h"

class TextureManager
{
public:
    static SDL_Texture* loadTexture(const char* filename, SDL_Renderer* ren);
    static void drawTexture(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst, SDL_Renderer* ren);
};

#endif // TEXTUREMANAGER_H
