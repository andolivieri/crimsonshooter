#include "trailcomponent.h"
#include "engine/game.h"
#include "engine/texturemanager.h"
#include <algorithm>
#include <cmath>

TrailComponent::TrailComponent(Config c)
    : color(c.color), maxPoints(c.maxPoints), thickness(c.thickness)
{
}

TrailComponent& TrailComponent::setColor(SDL_Color c) { color = c; return *this; }
TrailComponent& TrailComponent::setMaxPoints(int n) { maxPoints = n; return *this; }
TrailComponent& TrailComponent::setThickness(int t) { thickness = t; return *this; }

void TrailComponent::init()
{
    transform = &entity->getComponent<TransformComponent>();
}

void TrailComponent::update()
{
    if(!transform) return;
    points.push_front(transform->center());
    while(static_cast<int>(points.size()) > maxPoints)
        points.pop_back();
}

void TrailComponent::draw()
{
    if(points.size() < 2) return;

    SDL_SetRenderDrawBlendMode(TextureManager::renderer, SDL_BLENDMODE_BLEND);

    const int segments = static_cast<int>(points.size()) - 1;
    for(int i = 0; i < segments; i++)
    {
        // head (i == 0) is brightest and thickest, tail fades to transparent
        float t = 1.0f - (static_cast<float>(i) / segments);
        Uint8 a = static_cast<Uint8>(color.a * t);

        float x1 = points[i].x - Game::camera.x;
        float y1 = points[i].y - Game::camera.y;
        float x2 = points[i + 1].x - Game::camera.x;
        float y2 = points[i + 1].y - Game::camera.y;

        // perpendicular unit vector to give the segment some width
        float dx = x2 - x1;
        float dy = y2 - y1;
        float len = std::sqrt(dx * dx + dy * dy);
        float nx = 0, ny = 0;
        if(len > 0.0001f) { nx = -dy / len; ny = dx / len; }

        int halfThick = std::max(1, static_cast<int>(std::round(thickness * t)));

        SDL_SetRenderDrawColor(TextureManager::renderer, color.r, color.g, color.b, a);
        for(int k = -halfThick + 1; k < halfThick; k++)
        {
            SDL_RenderDrawLine(TextureManager::renderer,
                static_cast<int>(x1 + nx * k), static_cast<int>(y1 + ny * k),
                static_cast<int>(x2 + nx * k), static_cast<int>(y2 + ny * k));
        }
    }

    SDL_SetRenderDrawBlendMode(TextureManager::renderer, SDL_BLENDMODE_NONE);
}
