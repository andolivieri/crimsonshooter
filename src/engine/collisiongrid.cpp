#include "engine/collisiongrid.h"
#include <algorithm>
#include "ecs/ecs.h"
#include "ecs/base/collidercomponent.h"

int64_t CollisionGrid::key(int cx, int cy)
{
    return (static_cast<int64_t>(cx) << 32) | static_cast<uint32_t>(cy);
}

void CollisionGrid::rebuild(const std::vector<Entity*>& colliders)
{
    m_cells.clear();
    for(Entity* e : colliders)
    {
        if(!e->hasComponent<ColliderComponent>())
            continue;
        const SDL_Rect& r = e->getComponent<ColliderComponent>().collider;
        const int c0 = r.x / CELL_SIZE, c1 = (r.x + r.w) / CELL_SIZE;
        const int r0 = r.y / CELL_SIZE, r1 = (r.y + r.h) / CELL_SIZE;
        for(int cy = r0; cy <= r1; ++cy)
            for(int cx = c0; cx <= c1; ++cx)
                m_cells[key(cx, cy)].push_back(e);
    }
}

std::vector<Entity*> CollisionGrid::query(int x, int y, int w, int h) const
{
    std::vector<Entity*> result;
    const int c0 = x / CELL_SIZE, c1 = (x + w) / CELL_SIZE;
    const int r0 = y / CELL_SIZE, r1 = (y + h) / CELL_SIZE;
    for(int cy = r0; cy <= r1; ++cy)
    {
        for(int cx = c0; cx <= c1; ++cx)
        {
            auto it = m_cells.find(key(cx, cy));
            if(it == m_cells.end())
                continue;
            for(Entity* e : it->second)
                result.push_back(e);
        }
    }
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}
