#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <map>
#include <string>

class AssetManager
{
public:
    static SDL_Texture* getTexture(const std::string& path);
    static Mix_Chunk* getSound(const std::string& path);
    static TTF_Font* getFont(const std::string& path);
private:

    static std::map<std::string, SDL_Texture*> textures;
    static std::map<std::string, Mix_Chunk*> sounds;
    static std::map<std::string, TTF_Font*> fonts;
};

#endif // ASSETMANAGER_H
