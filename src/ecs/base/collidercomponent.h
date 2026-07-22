#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <string>
#include <functional>
#include <vector>
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

    bool isSolid = false;
    bool blockedBySolids = false;

    ColliderComponent() {}
    ColliderComponent(const std::string& t): tag(t){}
    ColliderComponent(const std::string& t, int paddingX, int paddingY, float scale);

    ColliderComponent& onCollision(std::function<void(Entity& target)> p);

    ColliderComponent& setSolid(bool b = true) { isSolid = b; return *this; }
    ColliderComponent& setBlockedBySolids(std::vector<std::string> filters = {});
    ColliderComponent& setBlockedBySolids(const std::string& filter);
    bool isBlockedBy(const std::string& solidTag) const;

    void init() override;
    void update() override;
    void draw() override;


private:

    void resolveAgainstSolids();

    // Solid tags that filter blocking. Empty => blocked by all solids.
    // A "TAG" entry whitelists; a "!TAG" entry blacklists (blacklist wins).
    std::vector<std::string> blockFilters;

    std::function<void(Entity& target)> onCollisionCb;

};

#endif // COLLIDERCOMPONENT_H
