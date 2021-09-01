#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"

class TextComponent : public Component
{
public:

    TextComponent(const std::string& text): txt(text){}

    void init() override;
    void draw() override;


private:
    TransformComponent* transform;
    std::string txt;

};
#endif // TEXTCOMPONENT_H
