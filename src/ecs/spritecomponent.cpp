#include "spritecomponent.h"

SpriteComponent::SpriteComponent(const char *path)
{
    m_texture = TextureManager::loadTexture(path);
}

SpriteComponent::~SpriteComponent()
{
    SDL_DestroyTexture(m_texture);
}

void SpriteComponent::init()
{

    transform = &entity->getComponent<TransformComponent>();

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 32;
    srcRect.h = 32;

    _update();
}

void SpriteComponent::update()
{
    _update();
}

void SpriteComponent::draw()
{
    TextureManager::drawTexture(m_texture, srcRect, dstRect, transform->rotation);

#ifdef __DEBUG
    SDL_RenderDrawRect(TextureManager::renderer, &dstRect);
#endif
}

void SpriteComponent::_update()
{
    dstRect.x = (int)transform->pos.x;
    dstRect.y = (int)transform->pos.y;
    dstRect.w = transform->scaledWidth();
    dstRect.h = transform->scaledHeight();
}
