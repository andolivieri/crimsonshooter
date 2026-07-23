#ifndef SPARKCOMPONENT_H
#define SPARKCOMPONENT_H

#include <SDL.h>
#include <vector>
#include "ecs/ecs.h"
#include "helpers/vector2d.h"

class SparkComponent : public Component
{
public:

    struct Config {
        SDL_Color color = {170, 170, 170, 255};
        int   count = 16; 
        float sparkDuration = 900;       // spark particle duration
    };

    SparkComponent(Vector2D worldPos, Config = {});

    void init() override;
    void update() override;
    void draw() override;

private:
    struct Particle {
        Vector2D pos;
        Vector2D vel;
    };

    Vector2D m_origin;
    SDL_Color m_color;
    int m_count;
    std::vector<Particle> m_particles;
    uint32_t m_startTicks = 0;
    uint32_t m_lifeMsec = 2000;
};

#endif // SPARKCOMPONENT_H
