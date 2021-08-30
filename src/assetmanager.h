#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "SDL.h"
#include <map>

class AssetManager
{
public:
    static SDL_Texture* getTexture(const std::string& path);
private:

    static std::map<std::string, SDL_Texture*> textures;
};

#endif // ASSETMANAGER_H
