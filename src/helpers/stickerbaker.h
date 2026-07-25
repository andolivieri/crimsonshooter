#ifndef STICKERBAKER_H
#define STICKERBAKER_H

#include <SDL.h>
#include <cstdint>
#include <vector>

// deferred "stamp this sprite into the baked map"
struct StickerOp
{
    SDL_Texture* texture;
    SDL_Rect src;
    SDL_Rect worldDst;
    double rotation;
    SDL_RendererFlip flip;
    uint8_t alpha, r, g, b;
    int group;
    uint64_t seq; // enqueue order, tiebreak within the same group
};

// Collects StickerOps during a frame and stamps them all into a target texture
class StickerBaker
{
public:
    static void enqueue(const StickerOp& op);
    static void flush(SDL_Renderer* renderer, SDL_Texture* target);

private:
    static std::vector<StickerOp> s_pending;
    static uint64_t s_seq;
};

#endif // STICKERBAKER_H
