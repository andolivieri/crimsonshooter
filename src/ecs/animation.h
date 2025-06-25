#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>

const uint32_t ANIMATION_FLAG_HIDE_AFTER_END = 0x01; // Hide sprite after animation ends

struct Animation
{
    int col;
    int row;
    int frames;
    uint32_t speed;
    uint32_t flags;
};

#endif // ANIMATION_H
