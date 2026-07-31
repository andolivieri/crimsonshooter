#ifndef COLLISIONGRID_H
#define COLLISIONGRID_H

#include <vector>
#include <unordered_map>
#include <cstdint>

class Entity;

// broad-phase bucket for dynamic colliders (foes, bullets, players)
class CollisionGrid
{
public:
    void rebuild(const std::vector<Entity*>& colliders);
    std::vector<Entity*> query(int x, int y, int w, int h) const;

private:
    static constexpr int CELL_SIZE = 128;
    static int64_t key(int cx, int cy);

    std::unordered_map<int64_t, std::vector<Entity*>> m_cells;
};

#endif // COLLISIONGRID_H
