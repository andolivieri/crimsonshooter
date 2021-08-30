#include "spritecomponent.h"
#include "assetmanager.h"

SpriteComponent::SpriteComponent(const std::string& path)
{
    m_texture = AssetManager::getTexture(path);
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

SpriteComponent::SpriteComponent(const std::string& path, const SDL_Rect &src):
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


    if(m_animated && (m_animationIndex < m_animationLoops || m_animationLoops <= 0)){
        Animation a = m_animation[m_currentAnimation];

        int frame = static_cast<int>((SDL_GetTicks() / a.speed) % a.frames);

        if(frame == a.frames - 1)
            m_animationIndex++;


        srcRect.x = srcRect.w * frame;
        srcRect.y = a.index * srcRect.h;
    }

    TextureManager::drawTexture(m_texture, srcRect, dstRect, transform->rotation, flip);

#ifdef __DEBUG
    SDL_RenderDrawRect(TextureManager::renderer, &dstRect);
#endif
}

void SpriteComponent::play(const std::string &anim, int repeat)
{
    m_animated = true;
    if(anim != m_currentAnimation)
        m_animationIndex = 0;
    m_currentAnimation = anim;
    m_animationLoops = repeat;
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
