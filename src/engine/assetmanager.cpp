#include "engine/assetmanager.h"
#include "engine/texturemanager.h"
#include <iostream>

std::map<std::string, SDL_Texture*> AssetManager::textures;
std::map<std::string, Mix_Chunk*> AssetManager::sounds;
std::map<std::string, TTF_Font*> AssetManager::fonts;

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

TTF_Font *AssetManager::getFont(const std::string &path)
{
    if(!fonts.count(path))
    {
        TTF_Font* t = TTF_OpenFont(path.c_str(), 24);
        if(!t)
        {
            std::cout << "Error TTF_OpenFont: " << SDL_GetError();
        }
        fonts[path] = t;
    }

    return fonts[path];
}

