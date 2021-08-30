#ifndef BLOODPATCH_H
#define BLOODPATCH_H

#include <string>
#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"
#include "vector2d.h"

class BloodPatchComponent : public Component
{
public:

    BloodPatchComponent(const Vector2D& v);

    void init() override;
    void update() override;

    BloodPatchComponent &setStartSize(int w, int h);
    BloodPatchComponent& setMaxSize(int w, int h);


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
