#include "spritecomponent.h"
#include "engine/assetmanager.h"
#include "engine/game.h"

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

    SDL_Rect cameraDst = dstRect;
    if(!absolute){
        cameraDst.x = transform->pos.x - Game::camera.x;
        cameraDst.y = transform->pos.y - Game::camera.y;
    }

    // THIS is a mess already :(

    bool animationEnded = m_animated && m_animationLoops >= 0 && m_animLoopCounter == m_animationLoops;
    if(animationEnded && m_animation[m_currentAnimation].flags & ANIMATION_FLAG_HIDE_AFTER_END)
        return;

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

    TextureManager::drawTexture(m_texture, srcRect, cameraDst, transform->rotation, flip, alpha, colorR, colorG, colorB);

    if(_showFrame)
    {
        SDL_SetRenderDrawColor(TextureManager::renderer, 255, 255,255,1);
        SDL_RenderDrawRect(TextureManager::renderer, &cameraDst);
    }

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

SpriteComponent &SpriteComponent::setAbsolute(bool b)
{
    absolute = b;
    return *this;
}

SpriteComponent &SpriteComponent::showFrame(bool b)
{
    _showFrame = b;
    return *this;
}

SpriteComponent &SpriteComponent::addAnimation(const char *name, const Animation &a)
{
    auto aa = a;
    aa.flags = 0x0;
    m_animation.emplace(name, aa);
    return *this;
}

SpriteComponent &SpriteComponent::addAnimation(const char *name, const Animation &a, const uint32_t flags)
{
    auto aa = a;
    aa.flags = flags;
    m_animation.emplace(name, aa);
    return *this;
}

SpriteComponent &SpriteComponent::setAlpha(uint8_t a)
{
    alpha = a;
    return *this;
}

SpriteComponent &SpriteComponent::setColorMod(uint8_t r, uint8_t g, uint8_t b)
{
    colorR = r;
    colorG = g;
    colorB = b;
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
