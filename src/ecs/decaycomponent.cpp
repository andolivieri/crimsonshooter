#include "decaycomponent.h"
#include "spritecomponent.h"
#include <iostream>
#include <algorithm>

void DecayComponent::init()
{
    startTimeMsec = SDL_GetTicks();
}

void DecayComponent::update()
{
    auto timediff = SDL_GetTicks() - startTimeMsec;

    if(timediff < startFadeAfterMsec)
        return;

    auto reltimediff = timediff-startFadeAfterMsec;

    if(entity->hasComponent<SpriteComponent>()){
        auto& sprite = entity->getComponent<SpriteComponent>();
        int alpha = static_cast<int>((reltimediff*255.f) / (decayInMsec - startFadeAfterMsec));
        sprite.setAlpha(255 - std::min(255, alpha));
    }

    if(timediff > decayInMsec)
        entity->setActive(false);

}
