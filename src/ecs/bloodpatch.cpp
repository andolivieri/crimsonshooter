#include "bloodpatch.h"
#include "game.h"


BloodSplatComponent::BloodSplatComponent(const Vector2D &v):
    centeredIn(v)
{
    startSize = {32,32};
    maxSize = {64,64};

}

void BloodSplatComponent::init()
{
    if(!entity->hasComponent<TransformComponent>())
        entity->addComponent<TransformComponent>(centeredIn);

    if(!entity->hasComponent<SpriteComponent>())
        entity->addComponent<SpriteComponent>("assets/bloodpatch.png")
                .setSrcRect({0,0,128,128});


    transform = &entity->getComponent<TransformComponent>();
    entity->addGroup(groupBloodPatches);

    startTime = time(NULL);

}

void BloodSplatComponent::update()
{

    int increment = (time(NULL) - startTime) * 2;
    auto width = startSize.x + increment;
    auto height = startSize.y + increment;

    if(width < maxSize.x)
        transform->width = width;
    if(height < maxSize.y)
        transform->height = height;

    transform->pos.x = centeredIn.x - transform->width / 2.f;
    transform->pos.y = centeredIn.y - transform->height / 2.f;

}

BloodSplatComponent &BloodSplatComponent::setStartSize(int w, int h)
{
    startSize.x = w;
    startSize.y = h;
    return *this;
}

BloodSplatComponent &BloodSplatComponent::setMaxSize(int w, int h)
{
    maxSize.x = w;
    maxSize.y = h;
    return *this;
}

