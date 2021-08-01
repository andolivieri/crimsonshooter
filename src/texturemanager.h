#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "SDL.h"

class TextureManager
{
public:
    static SDL_Texture* loadTexture(const char* filename);
    static void drawTexture(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst);
    static SDL_Renderer* renderer;
};

#endif // TEXTUREMANAGER_H
