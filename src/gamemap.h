#ifndef MAP_H
#define MAP_H
#include "SDL.h"

#define MAP_COLS  30
#define MAP_ROWS 30
#define TILESIZE 32

class GameMap
{
public:
    GameMap(SDL_Renderer *ren);
    ~GameMap();

    enum {
        WATER,
        DIRT,
        GRASS
    };

    void LoadMap(int m[MAP_ROWS][MAP_COLS]);
    void DrawMap();

private:
    SDL_Rect m_srcRect, m_dstRect;
    SDL_Renderer* m_renderer;
    SDL_Texture* dirt;
    SDL_Texture* grass;
    SDL_Texture* water;

    int map[MAP_ROWS][MAP_COLS];
};

#endif // MAP_H
