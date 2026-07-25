#ifndef MAP_H
#define MAP_H
#include "SDL.h"

#define MAP_COLS  30
#define MAP_ROWS 30
#define TILESIZE 32

#include <string>
#include <vector>
#include "ecs/ecs.h"

struct MapLayer {
    std::string name;
    int id;
    bool visible;
    float opacity;
    std::string type; // "tilelayer", "objectgroup", "imagelayer", "group"
    std::vector<unsigned int> data; // For tile layers
    int width, height;
    int offsetX, offsetY; // Layer offset
};

// tiled custom prop variant
struct MapProperty {
    std::string name;
    std::string type; // "string", "int", "bool", "float"
    std::string sval;
    int ival = 0;
    bool bval = false;
    float fval = 0.f;
};

// object from an "objectgroup" layer (named rect + props)
struct MapObject {
    int id = 0;
    std::string name;
    std::string type; // Tiled "class"/"type" field
    SDL_Rect rect{0, 0, 0, 0}; // world coordinates (top-left origin)
    std::vector<MapProperty> properties;

    const MapProperty* prop(const std::string& n) const;
    bool hasProp(const std::string& n) const { return prop(n) != nullptr; }
    std::string getString(const std::string& n, const std::string& def = "") const;
    int getInt(const std::string& n, int def = 0) const;
    bool getBool(const std::string& n, bool def = false) const;
};

class GameMap
{
public:
    GameMap();
    ~GameMap();

    enum {
        WATER,
        DIRT,
        GRASS
    };

    static int mapWidth;
    static int mapHeight;
    static std::vector<MapLayer> layers;
    static std::vector<MapObject> objects;
    static SDL_Texture* staticLayerTexture;

    static void LoadMap(const std::string& path, EntityManager& em);
    static const MapObject* getObject(const std::string& name);

private:

};

#endif // MAP_H
