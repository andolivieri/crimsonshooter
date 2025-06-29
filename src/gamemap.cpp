#include "gamemap.h"
#include "texturemanager.h"

#include "game.h"
#include <fstream>
#include <stdint.h>
#include <iostream>
#include <json.hpp>
using json = nlohmann::json;

// Bits on the far end of the 32-bit global tile ID are used for tile flags
const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

int GameMap::mapWidth = 0;
int GameMap::mapHeight = 0;
std::vector<MapLayer> GameMap::layers;

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

    mapWidth = w * tilewidth;
    mapHeight = h * tileheight;

    // Load tileset information from JSON
    int firstgid = 1;
    int tilesetRows = 16;
    int tilesetCols = 16;
    
    if (j.contains("tilesets") && !j["tilesets"].empty()) {
        firstgid = j["tilesets"][0]["firstgid"];
        // todo andoli: Could extract tileset dimensions from .tsx file if needed
    }

    SDL_Texture* tilesetTexture = TextureManager::loadTexture("assets/maptiles.png");

    if (j.contains("layers")) {
        for (const auto& layer : j["layers"]) {

            if (layer["type"] != "tilelayer") {
                continue;
            }
            
            if (layer.contains("visible") && !layer["visible"]) {
                continue;
            }
            
            std::string layerName = layer.contains("name") ? layer["name"] : "Unnamed";
            std::cout << "Processing layer: " << layerName << std::endl;
            
            int layerWidth = layer["width"];
            int layerHeight = layer["height"];
            
            for(int y = 0; y < layerHeight; y++) {
                for(int x = 0; x < layerWidth; x++) {
                    unsigned int tileid = layer["data"][y * layerWidth + x];
                    
                    if (tileid == 0) {
                        continue;
                    }

                    bool flipped_horizontally = (tileid & FLIPPED_HORIZONTALLY_FLAG);
                    bool flipped_vertically = (tileid & FLIPPED_VERTICALLY_FLAG);
                    bool flipped_diagonally = (tileid & FLIPPED_DIAGONALLY_FLAG);

                    uint32_t flip = SDL_FLIP_NONE;

                    if(flipped_horizontally)
                        flip |= SDL_FLIP_HORIZONTAL;

                    if(flipped_vertically)
                        flip |= SDL_FLIP_VERTICAL;

                    if(flipped_diagonally)
                        flip |= SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL;

                    tileid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);

                    SDL_Rect src;
                    src.y = (static_cast<int>((tileid - firstgid) / tilesetRows)) * tileheight;
                    src.x = ((tileid - firstgid) % tilesetCols) * tilewidth;
                    src.w = tilewidth;
                    src.h = tileheight;

                    SDL_Rect dst;
                    dst.x = x * tilewidth;
                    dst.y = y * tileheight;
                    dst.w = tilewidth;
                    dst.h = tileheight;

                    Game::addTile(tilesetTexture, src, dst, (SDL_RendererFlip)flip);
                }
            }
        }
    }
}