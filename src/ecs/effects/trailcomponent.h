#ifndef TRAILCOMPONENT_H
#define TRAILCOMPONENT_H

#include "ecs/ecs.h"
#include "ecs/base/transformcomponent.h"
#include "helpers/vector2d.h"
#include <SDL.h>
#include <deque>

// fading trail that follows the host entity's TransformComponent.
class TrailComponent : public Component
{
public:
    TrailComponent(SDL_Color color = {80, 255, 80, 255}, int maxPoints = 14, int thickness = 3);

    TrailComponent& setColor(SDL_Color c);
    TrailComponent& setMaxPoints(int n);
    TrailComponent& setThickness(int t);

    void init() override;
    void update() override;
    void draw() override;

private:
    TransformComponent* transform = nullptr;
    std::deque<Vector2D> points;
    SDL_Color color;
    int maxPoints;
    int thickness;
};

#endif // TRAILCOMPONENT_H
