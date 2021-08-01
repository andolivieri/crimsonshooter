#include "texturemanager.h"
#include "SDL_image.h"

SDL_Renderer* TextureManager::renderer = 0;


SDL_Texture *TextureManager::loadTexture(const char *filename)
{
    SDL_Surface* tmp = IMG_Load(filename);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(TextureManager::renderer, tmp);
    SDL_FreeSurface(tmp);
    return tex;
}

void TextureManager::drawTexture(SDL_Texture *tex, SDL_Rect src, SDL_Rect dst)
{
    SDL_RenderCopy(TextureManager::renderer, tex, &src, &dst);
}
