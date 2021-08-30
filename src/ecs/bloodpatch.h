#ifndef BLOODPATCH_H
#define BLOODPATCH_H

#include <string>
#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"
#include "vector2d.h"

class BloodSplatComponent : public Component
{
public:

    BloodSplatComponent(const Vector2D& v);

    void init() override;
    void update() override;

    BloodSplatComponent &setStartSize(int w, int h);
    BloodSplatComponent& setMaxSize(int w, int h);


private:
    SDL_Point startSize;
    SDL_Point maxSize;

    Vector2D centeredIn;
    TransformComponent* transform;
    time_t startTime;
    int startRadius;
    int speed = 2;
    int maxRadius = 10;

};

#endif // BLOODPATCH_H
