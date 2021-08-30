#include "texturemanager.h"
#include "SDL_image.h"

SDL_Renderer* TextureManager::renderer = 0;


SDL_Texture *TextureManager::loadTexture(const std::string& filename)
{
    SDL_Surface* tmp = IMG_Load(filename.c_str());
    SDL_Texture* tex = SDL_CreateTextureFromSurface(TextureManager::renderer, tmp);
    SDL_FreeSurface(tmp);
    return tex;
}

void TextureManager::drawTexture(SDL_Texture *tex, SDL_Rect src, SDL_Rect dst, double rotation, SDL_RendererFlip flip)
{
    SDL_Point center;
    center.x = (dst.w) / 2;
    center.y = (dst.h) / 2;

    SDL_RenderCopyEx(TextureManager::renderer, tex, &src, &dst, rotation, &center, flip);

}
