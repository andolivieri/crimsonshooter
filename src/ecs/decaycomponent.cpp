#include "decaycomponent.h"
#include "spritecomponent.h"
#include <iostream>

void DecayComponent::init()
{
    startTimeMsec = SDL_GetTicks() ;
}

void DecayComponent::update()
{
    auto timediff = SDL_GetTicks() - startTimeMsec;

    if(entity->hasComponent<SpriteComponent>() &&   decayTimeMsec - timediff < dissolveInMsec){
        auto& sprite = entity->getComponent<SpriteComponent>();
        int alpha = static_cast<int>(timediff*255.f / dissolveInMsec);
        sprite.setAlpha(255 - alpha > 0 ? 255 -alpha : 0);
        // BUG qui
        //std::cout << "alpha:"   + std::to_string(255 -alpha) << " msec:" << timediff << std::endl;
    }


    if(timediff > decayTimeMsec)
        entity->setActive(false);

}
