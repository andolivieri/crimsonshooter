#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs/ecs.h"
#include "ecs/base/transformcomponent.h"

class TextComponent : public Component
{
public:

    std::string txt;

    SDL_Rect padding;
    float scale = 1;
    bool absolute = true;
    SDL_Color color = {255, 255, 255};

    TextComponent(const std::string& text = "", SDL_Rect padding={0,0,0,0}):
        txt(text), padding(padding){}

    void init() override;
    void draw() override;

    TextComponent& setAbsolute(bool v);
    TextComponent& setColor(SDL_Color c);


private:
    TransformComponent* transform;
};
#endif // TEXTCOMPONENT_H
