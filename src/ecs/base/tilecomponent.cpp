#include "tilecomponent.h"
#include "engine/game.h"
#include <iostream>

TileComponent::TileComponent(SDL_Texture* texture, const SDL_Rect& srcRect, SDL_RendererFlip flip)
    : m_texture(texture), m_srcRect(srcRect), m_flip(flip), m_transform(nullptr)
{
}

void TileComponent::init()
{
    if (entity->hasComponent<TransformComponent>()) {
        m_transform = &entity->getComponent<TransformComponent>();
    }
}

void TileComponent::update()
{
    
}

void TileComponent::draw()
{
    if (!m_texture || !m_transform) return;
    
    SDL_Rect cameraDst;
    cameraDst.x = static_cast<int>(m_transform->pos.x - Game::camera.x);
    cameraDst.y = static_cast<int>(m_transform->pos.y - Game::camera.y);
    cameraDst.w = m_transform->width;
    cameraDst.h = m_transform->height;
    
    // culling
    if (cameraDst.x + cameraDst.w > 0 && cameraDst.x < Game::camera.w &&
        cameraDst.y + cameraDst.h > 0 && cameraDst.y < Game::camera.h) {
        SDL_RenderCopyEx(Game::getRenderer(), m_texture, &m_srcRect, &cameraDst, 0, nullptr, m_flip);
    }
}