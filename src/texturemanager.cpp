#include "texturemanager.h"
#include "SDL_image.h"

SDL_Texture *TextureManager::loadTexture(const char *filename, SDL_Renderer *ren)
{
    SDL_Surface* tmp = IMG_Load(filename);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tmp);
    SDL_FreeSurface(tmp);
    return tex;
}
