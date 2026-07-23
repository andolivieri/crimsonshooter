#include "engine/gamemap.h"
#include "engine/texturemanager.h"
#include "engine/resources.h"
#include "engine/navgrid.h"

#include "engine/game.h"
#include <stdint.h>
#include <iostream>
#include <helpers/json.hpp>
#include "ecs/components.h"
using json = nlohmann::json;

namespace
{
    void parsePropertyValue(const json& value, MapProperty& mp)
    {
        if (mp.type == "int")
        {
            mp.ival = value.get<int>();
            mp.sval = std::to_string(mp.ival);
        }
        else if (mp.type == "bool")
        {
            mp.bval = value.get<bool>();
            mp.sval = mp.bval ? "true" : "false";
        }
        else if (mp.type == "float")
        {
            mp.fval = value.get<float>();
            mp.sval = std::to_string(mp.fval);
        }
        else
        {
            mp.sval = value.get<std::string>();
        }
    }

    bool boolProperty(const json& node, const std::string& name, bool def = false)
    {
        if (!node.contains("properties"))
        {
            return def;
        }
        for (const auto& p : node["properties"])
        {
            if (p.value("name", std::string{}) == name)
            {
                return p.value("value", def);
            }
        }
        return def;
    }

    // Parse a Tiled "objectgroup" layer into GameMap::objects. Each object
    // carries its name, type, rect, and custom properties (string/int/bool/float).
    void parseObjectLayer(const json& layer, std::vector<MapObject>& out)
    {
        if (!layer.contains("objects"))
        {
            return;
        }

        for (const auto& obj : layer["objects"])
        {
            MapObject mo;
            mo.id = obj.value("id", 0);
            mo.name = obj.value("name", std::string{});
            mo.type = obj.value("type", obj.value("class", std::string{}));
            mo.rect.x = static_cast<int>(obj.value("x", 0.0));
            mo.rect.y = static_cast<int>(obj.value("y", 0.0));
            mo.rect.w = static_cast<int>(obj.value("width", 0.0));
            mo.rect.h = static_cast<int>(obj.value("height", 0.0));

            if (obj.contains("properties"))
            {
                for (const auto& p : obj["properties"])
                {
                    MapProperty mp;
                    mp.name = p.value("name", std::string{});
                    mp.type = p.value("type", std::string{"string"});

                    parsePropertyValue(p["value"], mp);
                    mo.properties.push_back(mp);
                }
            }

            std::cout << "Parsed object: '" << mo.name << "' type='" << mo.type
                      << "' (" << mo.rect.x << "," << mo.rect.y << ")" << std::endl;
            out.push_back(mo);
        }
    }
}

// Bits on the far end of the 32-bit global tile ID are used for tile flags
const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

int GameMap::mapWidth = 0;
int GameMap::mapHeight = 0;
std::vector<MapLayer> GameMap::layers;
std::vector<MapObject> GameMap::objects;

const MapProperty* MapObject::prop(const std::string& n) const
{
    for (const auto& p : properties)
    {
        if (p.name == n)
        {
            return &p;
        }
    }
    return nullptr;
}

std::string MapObject::getString(const std::string& n, const std::string& def) const
{
    const MapProperty* p = prop(n);
    return p ? p->sval : def;
}

int MapObject::getInt(const std::string& n, int def) const
{
    const MapProperty* p = prop(n);
    return p ? p->ival : def;
}

bool MapObject::getBool(const std::string& n, bool def) const
{
    const MapProperty* p = prop(n);
    return p ? p->bval : def;
}

const MapObject* GameMap::getObject(const std::string& name)
{
    for (const auto& o : objects)
    {
        if (o.name == name)
        {
            return &o;
        }
    }
    return nullptr;
}

GameMap::GameMap()
{

}

GameMap::~GameMap()
{

}


void GameMap::LoadMap(const std::string &path, EntityManager& em)
{
    // Statics persist across LoadMap calls; reset so reloading a different
    // level doesn't accumulate the previous map's layers/objects.
    layers.clear();
    objects.clear();

    json j = json::parse(Resources::readString(path));

    int w = j["width"];
    int h = j["height"];
    int tilewidth = j["tilewidth"];
    int tileheight = j["tileheight"];

    mapWidth = w * tilewidth;
    mapHeight = h * tileheight;

    NavGrid::reset(w, h, tilewidth, tileheight);

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

            if (layer.contains("visible") && !layer["visible"]) {
                continue;
            }

            if (layer["type"] == "objectgroup") {
                parseObjectLayer(layer, objects);
                continue;
            }

            if (layer["type"] != "tilelayer") {
                continue;
            }

            std::string layerName = layer.contains("name") ? layer["name"] : "Unnamed";

            bool solidLayer = boolProperty(layer, "collides") || layerName == "COLLIDERS";
            std::cout << "Processing layer: " << layerName
                      << (solidLayer ? " (solid)" : "") << std::endl;

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

                    //Game::addTile(tilesetTexture, src, dst, (SDL_RendererFlip)flip);
                     // Create tile entity with TileComponent
                    auto& tile = em.addEntity();
                    tile.addComponent<TransformComponent>(dst.x, dst.y, dst.w, dst.h);
                    tile.addComponent<TileComponent>(tilesetTexture, src, (SDL_RendererFlip)flip);
                    tile.addGroup(groupMap);

                    if (solidLayer)
                    {
                        tile.addComponent<ColliderComponent>(layerName).setSolid();
                        NavGrid::setBlocked(x, y);
                    }
                }
            }
        }
    }
}