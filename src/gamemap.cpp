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
    int tilesetRows = 14;
    int tilesetCols = 14;
    
    if (j.contains("tilesets") && !j["tilesets"].empty()) {
        firstgid = j["tilesets"][0]["firstgid"];
        // Note: Could extract tileset dimensions from .tsx file if needed
    }

    SDL_Texture* tilesetTexture = TextureManager::loadTexture("assets/maptiles.png");

    // Process all layers
    if (j.contains("layers")) {
        for (const auto& layer : j["layers"]) {
            // Skip non-tile layers (object layers, image layers, etc.)
            if (layer["type"] != "tilelayer") {
                continue;
            }
            
            // Skip invisible layers
            if (layer.contains("visible") && !layer["visible"]) {
                continue;
            }
            
            std::string layerName = layer.contains("name") ? layer["name"] : "Unnamed";
            std::cout << "Processing layer: " << layerName << std::endl;
            
            // Get layer dimensions (should match map dimensions for tile layers)
            int layerWidth = layer["width"];
            int layerHeight = layer["height"];
            
            // Process tiles in this layer
            for(int y = 0; y < layerHeight; y++) {
                for(int x = 0; x < layerWidth; x++) {
                    unsigned int tileid = layer["data"][y * layerWidth + x];
                    
                    // Skip empty tiles (tileid 0)
                    if (tileid == 0) {
                        continue;
                    }

                    // Read out the flags
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

                    // Clear the flags
                    tileid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);

                    // Calculate source rectangle in tileset
                    SDL_Rect src;
                    src.y = (static_cast<int>((tileid - firstgid) / tilesetRows)) * tileheight;
                    src.x = ((tileid - firstgid) % tilesetCols) * tilewidth;
                    src.w = tilewidth;
                    src.h = tileheight;

                    // Calculate destination rectangle on screen
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

void GameMap::LoadMapWithLayers(const std::string &path)
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

    // Clear existing layers
    layers.clear();

    // Load tileset information from JSON
    int firstgid = 1;
    int tilesetRows = 14;
    int tilesetCols = 14;
    
    if (j.contains("tilesets") && !j["tilesets"].empty()) {
        firstgid = j["tilesets"][0]["firstgid"];
    }

    SDL_Texture* tilesetTexture = TextureManager::loadTexture("assets/maptiles.png");

    // Process all layers and store layer information
    if (j.contains("layers")) {
        for (const auto& layerJson : j["layers"]) {
            MapLayer layer;
            layer.name = layerJson.contains("name") ? layerJson["name"] : "Unnamed";
            layer.id = layerJson.contains("id") ? layerJson["id"] : 0;
            layer.visible = layerJson.contains("visible") ? layerJson["visible"] : true;
            layer.opacity = layerJson.contains("opacity") ? layerJson["opacity"] : 1.0f;
            layer.type = layerJson.contains("type") ? layerJson["type"] : "unknown";
            layer.width = layerJson.contains("width") ? layerJson["width"] : w;
            layer.height = layerJson.contains("height") ? layerJson["height"] : h;
            layer.offsetX = layerJson.contains("x") ? layerJson["x"] : 0;
            layer.offsetY = layerJson.contains("y") ? layerJson["y"] : 0;

            // Store layer data for tile layers
            if (layer.type == "tilelayer" && layerJson.contains("data")) {
                layer.data.clear();
                for (const auto& tileId : layerJson["data"]) {
                    layer.data.push_back(tileId);
                }
            }

            layers.push_back(layer);

            // Skip non-tile layers for rendering
            if (layer.type != "tilelayer") {
                std::cout << "Skipping non-tile layer: " << layer.name << " (type: " << layer.type << ")" << std::endl;
                continue;
            }
            
            // Skip invisible layers
            if (!layer.visible) {
                std::cout << "Skipping invisible layer: " << layer.name << std::endl;
                continue;
            }
            
            std::cout << "Processing layer: " << layer.name << " (id: " << layer.id << ", opacity: " << layer.opacity << ")" << std::endl;
            
            // Process tiles in this layer
            for(int y = 0; y < layer.height; y++) {
                for(int x = 0; x < layer.width; x++) {
                    unsigned int tileid = layer.data[y * layer.width + x];
                    
                    // Skip empty tiles (tileid 0)
                    if (tileid == 0) {
                        continue;
                    }

                    // Read out the flags
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

                    // Clear the flags
                    tileid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);

                    // Calculate source rectangle in tileset
                    SDL_Rect src;
                    src.y = (static_cast<int>((tileid - firstgid) / tilesetRows)) * tileheight;
                    src.x = ((tileid - firstgid) % tilesetCols) * tilewidth;
                    src.w = tilewidth;
                    src.h = tileheight;

                    // Calculate destination rectangle on screen
                    SDL_Rect dst;
                    dst.x = (x * tilewidth) + layer.offsetX;
                    dst.y = (y * tileheight) + layer.offsetY;
                    dst.w = tilewidth;
                    dst.h = tileheight;

                    Game::addTile(tilesetTexture, src, dst, (SDL_RendererFlip)flip);
                }
            }
        }
    }

    std::cout << "Loaded " << layers.size() << " layers from map: " << path << std::endl;
}

