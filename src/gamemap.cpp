#include "gamemap.h"
#include "texturemanager.h"

#include "game.h"
#include <fstream>
#include <stdint.h>
#include <json.hpp>
using json = nlohmann::json;

// Bits on the far end of the 32-bit global tile ID are used for tile flags
const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

GameMap::GameMap()
{

}

GameMap::~GameMap()
{

}


void GameMap::LoadMap(const std::string &path)
{

    std::ifstream i(path);
    json j;
    i >> j;

    int w = j["width"];
    int h = j["height"];
    int tilewidth = j["tilewidth"];
    int tileheight = j["tileheight"];

    SDL_Texture* tilesetTexture = TextureManager::loadTexture("assets/maptiles.png");

    for(int y=0; y < h; y++){
        for(int x=0; x < w; x++){
            unsigned int tileid = j["layers"][0]["data"][y*w + x];

            // Read out the flags
            bool flipped_horizontally = (tileid & FLIPPED_HORIZONTALLY_FLAG);
            bool flipped_vertically = (tileid & FLIPPED_VERTICALLY_FLAG);
            bool flipped_diagonally = (tileid & FLIPPED_DIAGONALLY_FLAG);

            uint32_t flip = SDL_FLIP_NONE;

            if(flipped_horizontally)
                flip |=  SDL_FLIP_HORIZONTAL;


            if(flipped_vertically)
                flip |= SDL_FLIP_VERTICAL;


            //if(flipped_diagonally)
            //    flip |= SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL;

            // Clear the flags
            tileid &= ~(FLIPPED_HORIZONTALLY_FLAG |
                                FLIPPED_VERTICALLY_FLAG |
                                FLIPPED_DIAGONALLY_FLAG);
            int firstgid = 1;

            SDL_Rect src;
            src.y = (static_cast<int>((tileid - firstgid) / 16)) * tileheight;
            src.x = ((tileid-firstgid) % 16) * tilewidth;

            src.w = tilewidth;
            src.h = tileheight;

            SDL_Rect dst;
            dst.x = x*tilewidth;
            dst.y = y*tileheight;
            dst.w = tilewidth;
            dst.h = tileheight;



            Game::addTile(tilesetTexture, src, dst, (SDL_RendererFlip)flip);
        }
    }


}

