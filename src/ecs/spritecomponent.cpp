#include "spritecomponent.h"

SpriteComponent::SpriteComponent(const char *path)
{
    m_texture = TextureManager::loadTexture(path);
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 32;
    srcRect.h = 32;
}

SpriteComponent::SpriteComponent(SDL_Texture *t)
{
    m_texture = t;
}

SpriteComponent::SpriteComponent(SDL_Texture *t, const SDL_Rect &src, SDL_RendererFlip f):
    SpriteComponent(t)
{
    srcRect = src;
    flip = f;
}

SpriteComponent::SpriteComponent(const char *path, const SDL_Rect &src):
    SpriteComponent(path)
{
    srcRect = src;
}

SpriteComponent::~SpriteComponent()
{
    //SDL_DestroyTexture(m_texture);
}

void SpriteComponent::init()
{

    transform = &entity->getComponent<TransformComponent>();


    _update();
}

void SpriteComponent::update()
{
    _update();
}

void SpriteComponent::draw()
{

    if(m_animated){
        Animation a = m_animation[m_currentAnimation];
        srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / a.speed) % a.frames);
        srcRect.y = a.index * srcRect.h;
    }

    TextureManager::drawTexture(m_texture, srcRect, dstRect, transform->rotation, flip);

#ifdef __DEBUG
    SDL_RenderDrawRect(TextureManager::renderer, &dstRect);
#endif
}

void SpriteComponent::play(const std::string &anim)
{
    m_animated = true;
    m_currentAnimation = anim;
}

void SpriteComponent::stop()
{
    m_animated = false;
}

SpriteComponent &SpriteComponent::setSrcRect(const SDL_Rect &s)
{
    srcRect = s;
    return *this;
}

SpriteComponent &SpriteComponent::addAnimation(const char *name, const Animation &a)
{
    m_animation.emplace(name, a);
    return *this;
}


void SpriteComponent::_update()
{
    dstRect.x = (int)transform->pos.x;
    dstRect.y = (int)transform->pos.y;
    dstRect.w = transform->scaledWidth();
    dstRect.h = transform->scaledHeight();
}
