#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#include "SDL.h"
#include "ecs.h"
#include "transformcomponent.h"
#include "texturemanager.h"

class SpriteComponent : public Component
{
private:
    TransformComponent* transform;
    SDL_Texture *m_texture;
    SDL_Rect srcRect, dstRect;


    void _update();

public:
    SpriteComponent() = default;

    SpriteComponent(const char* path)
    {
        m_texture = TextureManager::loadTexture(path);
    }

    ~SpriteComponent()
    {
        SDL_DestroyTexture(m_texture);
    }


    void init() override
    {

        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = 32;
        srcRect.h = 32;

        _update();
    }

    void update() override
    {
        _update();
    }

    void draw() override
    {
        TextureManager::drawTexture(m_texture, srcRect, dstRect);
    }
};

#endif // SPRITECOMPONENT_H
