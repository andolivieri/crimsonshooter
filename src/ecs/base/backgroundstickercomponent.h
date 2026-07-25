#ifndef BACKGROUNDSTICKERCOMPONENT_H
#define BACKGROUNDSTICKERCOMPONENT_H

#include <cstdint>
#include "ecs/ecs.h"

class SpriteComponent;

// Stamps the entity's sprite into the baked static map when the entity is destroyed
class BackgroundStickerComponent : public Component
{
public:

    explicit BackgroundStickerComponent(int group);

    void unmount() override;

private:
    int m_group;
};

#endif // BACKGROUNDSTICKERCOMPONENT_H
