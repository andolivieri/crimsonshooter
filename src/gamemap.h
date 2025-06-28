#ifndef MAP_H
#define MAP_H
#include "SDL.h"

#define MAP_COLS  30
#define MAP_ROWS 30
#define TILESIZE 32

#include <string>
#include <vector>

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

    static void LoadMap(const std::string& path);
    static void LoadMapWithLayers(const std::string& path);

private:

};

#endif // MAP_H
