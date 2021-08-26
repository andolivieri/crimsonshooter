#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <string>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"
#include "spritecomponent.h"
#include "../game.h"


class TileComponent : public Component
{
public:
    TransformComponent* transform;
    SpriteComponent* sprite;

    SDL_Rect tileRect;
    int tileID;
    std::string path;

    TileComponent(){
        memset(&tileRect, 0, sizeof(tileRect));
    }

    TileComponent(int x,int y,int w,int h,int id)
    {
        tileRect.x = x;
        tileRect.y = y;
        tileRect.w = w;
        tileRect.h = h;
        tileID = id;

        switch (tileID) {
        case 0:
            path = "assets/water.png";
            break;
        case 1:
            path = "assets/dirt.png";
            break;
        case 2:
            path = "assets/grass.png";
            break;
        default:
            break;
        }
    }

    void init() override
    {
        entity->addComponent<TransformComponent>(tileRect.x,
                                                 tileRect.y,
                                                 tileRect.w,
                                                 tileRect.h);
        transform = &entity->getComponent<TransformComponent>();
        entity->addComponent<SpriteComponent>(path.c_str());
        sprite = &entity->getComponent<SpriteComponent>();
    }
};

#endif // TILECOMPONENT_H
