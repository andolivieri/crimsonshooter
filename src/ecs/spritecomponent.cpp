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

    transform = &entity->emplaceComponent<TransformComponent>();

    _update();
}

void SpriteComponent::update()
{
    _update();
}

void SpriteComponent::draw()
{

    // THIS is a mess already :(

    if(m_animated && (m_animLoopCounter < m_animationLoops || m_animationLoops <= 0)){
        Animation a = m_animation[m_currentAnimation];


        if(SDL_GetTicks() - lastTick > a.speed)
        {
            m_animCurrentFrameIndex = (m_animCurrentFrameIndex + 1) % a.frames;
            lastTick = SDL_GetTicks();
        }

        if(m_animCurrentFrameIndex == a.frames - 1)
            m_animLoopCounter++;


        srcRect.x = srcRect.w * m_animCurrentFrameIndex + a.col*srcRect.w;
        srcRect.y = a.row * srcRect.h;
    }

    TextureManager::drawTexture(m_texture, srcRect, dstRect, transform->rotation, flip, alpha);

#if 0
    SDL_SetRenderDrawColor(TextureManager::renderer, 255, 255,255,1);
    SDL_RenderDrawRect(TextureManager::renderer, &dstRect);
#endif
}

void SpriteComponent::play(const std::string &anim, int repeat)
{
    m_animated = true;
    if(anim != m_currentAnimation){
        m_animLoopCounter = 0;
        m_animCurrentFrameIndex = 0;
    }
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

SpriteComponent &SpriteComponent::setAlpha(uint8_t a)
{
    alpha = a;
    return *this;
}

SpriteComponent &SpriteComponent::setTransform(TransformComponent *t)
{
    transform = t;
    return *this;
}


void SpriteComponent::_update()
{
    dstRect.x = (int)transform->pos.x;
    dstRect.y = (int)transform->pos.y;
    dstRect.w = transform->scaledWidth();
    dstRect.h = transform->scaledHeight();
}
