#include "assetmanager.h"
#include "texturemanager.h"

std::map<std::string, SDL_Texture*> AssetManager::textures;
std::map<std::string, Mix_Chunk*> AssetManager::sounds;

SDL_Texture *AssetManager::getTexture(const std::string &path)
{
    if(!textures.count(path))
    {
        SDL_Texture* t = TextureManager::loadTexture(path);
        textures[path] = t;
    }

    return textures[path];
}

Mix_Chunk *AssetManager::getSound(const std::string &path)
{
    if(!sounds.count(path))
    {
        Mix_Chunk* t = Mix_LoadWAV(path.c_str());
        sounds[path] = t;
    }

    return sounds[path];
}

