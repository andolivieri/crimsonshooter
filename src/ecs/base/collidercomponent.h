#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <string>
#include <functional>
#include <SDL.h>
#include "ecs/ecs.h"
#include "transformcomponent.h"

class ColliderComponent : public Component
{
public:
    int paddingX = 0;
    int paddingY = 0;
    float scale = 1;
    SDL_Rect collider;
    std::string tag;
    TransformComponent* transform;

    // A solid collider is a static obstacle (e.g. a wall tile): it is visible to
    // other colliders' scans but never moves and never reacts on its own.
    bool isSolid = false;
    // A mover flagged blockedBySolids gets per-axis movement resolution against
    // every isSolid collider (player, foes) so it can't walk through walls.
    bool blockedBySolids = false;

    ColliderComponent() {}
    ColliderComponent(const std::string& t): tag(t){}
    ColliderComponent(const std::string& t, int paddingX, int paddingY, float scale);

    ColliderComponent& onCollision(std::function<void(Entity& target)> p);
    ColliderComponent& setSolid(bool b = true) { isSolid = b; return *this; }
    ColliderComponent& setBlockedBySolids(bool b = true) { blockedBySolids = b; return *this; }

    void init() override;
    void update() override;
    void draw() override;


private:

    void resolveAgainstSolids();

    std::function<void(Entity& target)> onCollisionCb;

};

#endif // COLLIDERCOMPONENT_H
