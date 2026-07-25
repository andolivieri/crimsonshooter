#include "stickerbaker.h"
#include "engine/texturemanager.h"

#include <algorithm>

std::vector<StickerOp> StickerBaker::s_pending;
uint64_t StickerBaker::s_seq = 0;

void StickerBaker::enqueue(const StickerOp& op)
{
    StickerOp copy = op;
    copy.seq = s_seq++;
    s_pending.push_back(copy);
}

void StickerBaker::flush(SDL_Renderer* renderer, SDL_Texture* target)
{
    if(s_pending.empty())
        return;

    if(!target || !renderer)
    {
        s_pending.clear();
        return;
    }

    std::stable_sort(s_pending.begin(), s_pending.end(),
                     [](const StickerOp& a, const StickerOp& b)
                     {
                         if(a.group != b.group) return a.group < b.group;
                         return a.seq < b.seq;
                     });

    SDL_Texture* prevTarget = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, target);

    for(const auto& op : s_pending)
    {
        TextureManager::drawTexture(op.texture, op.src, op.worldDst, op.rotation,
                                    op.flip, op.alpha, op.r, op.g, op.b);
    }

    SDL_SetRenderTarget(renderer, prevTarget);
    s_pending.clear();
}
