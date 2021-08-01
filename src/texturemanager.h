#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "SDL.h"

class TextureManager
{
public:
    static SDL_Texture* loadTexture(const char* filename, SDL_Renderer* ren);
};

#endif // TEXTUREMANAGER_H
