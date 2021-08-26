#ifndef MAP_H
#define MAP_H
#include "SDL.h"

#define MAP_COLS  30
#define MAP_ROWS 30
#define TILESIZE 32

#include <string>

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

    static void LoadMap(const std::string& path);

private:

};

#endif // MAP_H
