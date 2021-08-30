#include "assetmanager.h"
#include "texturemanager.h"

std::map<std::string, SDL_Texture*> AssetManager::textures;

SDL_Texture *AssetManager::getTexture(const std::string &path)
{
    if(!textures.count(path))
    {
        SDL_Texture* t = TextureManager::loadTexture(path);
        textures[path] = t;
    }

    return textures[path];
}
