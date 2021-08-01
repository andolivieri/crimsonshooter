#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#include "SDL.h"
#include "ecs.h"
#include "transformcomponent.h"
#include "texturemanager.h"

class SpriteComponent : public Component
{
private:
    TransformComponent* m_position;
    SDL_Texture *m_texture;
    SDL_Rect srcRect, dstRect;

public:
    SpriteComponent() = default;
    SpriteComponent(const char* path)
    {
        m_texture = TextureManager::loadTexture(path);
    }

    void init() override
    {

        m_position = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = srcRect.h = 32;
        dstRect.w = dstRect.h = 64;
    }

    void update() override
    {
        dstRect.x = m_position->x;
        dstRect.y = m_position->y;
    }

    void draw() override
    {
        TextureManager::drawTexture(m_texture, srcRect, dstRect);
    }
};

#endif // SPRITECOMPONENT_H
