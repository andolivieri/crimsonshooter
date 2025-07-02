#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "SDL.h"
#include <string>

class TextureManager
{
public:
    static SDL_Texture* loadTexture(const std::string& filename);
    static void drawTexture(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst, double rotation, SDL_RendererFlip flip, uint8_t alpha);
    static SDL_Renderer* renderer;
};

#endif // TEXTUREMANAGER_H
