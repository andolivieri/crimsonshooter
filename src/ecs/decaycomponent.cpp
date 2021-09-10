#include "decaycomponent.h"
#include "spritecomponent.h"

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
        sprite.setAlpha(255 - alpha);
    }


    if(timediff > decayTimeMsec)
        entity->setActive(false);

}
