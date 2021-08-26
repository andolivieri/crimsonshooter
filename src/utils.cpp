#include "utils.h"
#include <stdint.h>

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
