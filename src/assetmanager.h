#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "SDL.h"
#include "SDL_mixer.h"
#include <map>

class AssetManager
{
public:
    static SDL_Texture* getTexture(const std::string& path);
    static Mix_Chunk* getSound(const std::string& path);
private:

    static std::map<std::string, SDL_Texture*> textures;
    static std::map<std::string, Mix_Chunk*> sounds;
};

#endif // ASSETMANAGER_H
