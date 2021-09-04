#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"

class TextComponent : public Component
{
public:

    SDL_Rect padding;
    float scale = 1;

    TextComponent(const std::string& text, SDL_Rect padding={0,0,32,32}):
        txt(text), padding(padding){}

    void init() override;
    void draw() override;


private:
    TransformComponent* transform;
    std::string txt;

};
#endif // TEXTCOMPONENT_H
