#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "SDL.h"

class GameObject
{
public:
    GameObject(const char* textureSheet, SDL_Renderer* ren);
    ~GameObject();

    void update();
    void render();


    int x;
    int y;

private:
    SDL_Texture* m_texture;
    SDL_Rect m_srcRect, m_dstRect;
    SDL_Renderer* m_renderer;
};

#endif // GAMEOBJECT_H
