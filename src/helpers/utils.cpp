#include "helpers/utils.h"
#include <stdint.h>
#include <string>

void drawCircle(SDL_Renderer *renderer, SDL_Point center, int radius, SDL_Color color)
{
    SDL_Color c;


    SDL_GetRenderDrawColor(renderer, &c.r, &c.g, &c.b, &c.a);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

}

std::string strLeftPad(const std::string &str, const size_t num, const char paddingChar)
{
    std::string out(str);
    if(num > out.size())
        out.insert(0, num - out.size(), paddingChar);
    return out;
}

std::string strRightPad(const std::string &str, const size_t num, const char paddingChar)
{
    std::string out(str);
    if(num > out.size())
        out += paddingChar;
    return out;
}
