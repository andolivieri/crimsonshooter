#ifndef COLLISIONGRIDCOMPONENT_H
#define COLLISIONGRIDCOMPONENT_H

#include <cstdint>
#include <vector>
#include "ecs/ecs.h"
#include "engine/collisiongrid.h"


class CollisionGridComponent : public Component
{
public:
    static CollisionGridComponent& get(EntityManager& manager);

    std::vector<Entity*> query(int x, int y, int w, int h);

private:
    CollisionGrid m_grid;
    uint64_t m_builtTick = static_cast<uint64_t>(-1); // sentinel: never equals a real Game::tick
};

#endif // COLLISIONGRIDCOMPONENT_H
