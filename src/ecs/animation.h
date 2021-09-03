#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>

struct Animation
{
    int col;
    int row;
    int frames;
    uint32_t speed;
};

#endif // ANIMATION_H
