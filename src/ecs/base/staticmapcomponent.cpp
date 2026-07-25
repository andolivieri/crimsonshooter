#include "staticmapcomponent.h"
#include "engine/game.h"

StaticMapComponent::StaticMapComponent(SDL_Texture* texture)
    : m_texture(texture)
{
}

void StaticMapComponent::draw()
{
    if (!m_texture) return;

    SDL_Rect src = Game::camera;
    SDL_Rect dst = {0, 0, Game::camera.w, Game::camera.h};
    SDL_RenderCopy(Game::getRenderer(), m_texture, &src, &dst);
}
