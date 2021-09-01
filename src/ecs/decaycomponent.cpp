#include "decaycomponent.h"
#include "spritecomponent.h"

void DecayComponent::init()
{
    startTime = time(NULL);
}

void DecayComponent::update()
{
    auto timediff = time(NULL) - startTime;

    if(entity->hasComponent<SpriteComponent>() &&   decayTime - timediff < 5){
        auto& sprite = entity->getComponent<SpriteComponent>();
        int alpha = static_cast<int>(timediff*255.f / 5);
        sprite.setAlpha(255 - alpha);
    }


    if(timediff > decayTime)
        entity->setActive(false);

}
