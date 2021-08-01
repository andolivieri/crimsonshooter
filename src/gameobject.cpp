#include "gameobject.h"
#include "texturemanager.h"


GameObject::GameObject(const char *textureSheet, SDL_Renderer *ren):
    m_renderer(ren)
{
    m_texture = TextureManager::loadTexture(textureSheet);

    x = 0;
    y = 0;

    m_srcRect.h = 32;
    m_srcRect.w = 32;
    m_srcRect.x = 0;
    m_srcRect.y = 0;


}

void GameObject::update()
{
    m_dstRect.x = x;
    m_dstRect.y = y;
    m_dstRect.w = m_srcRect.w * 2;
    m_dstRect.h = m_srcRect.h * 2;

}

void GameObject::render()
{
    SDL_RenderCopy(m_renderer, m_texture, &m_srcRect, &m_dstRect);
}
