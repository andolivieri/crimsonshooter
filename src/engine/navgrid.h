#ifndef NAVGRID_H
#define NAVGRID_H

#include <vector>
#include <cstdint>
#include "helpers/vector2d.h"

class ColliderComponent;

// tile-aligned walkability grid for pathfinding.
class NavGrid
{
public:
    static void reset(int cols, int rows, int tileW, int tileH);
    static void setBlocked(int c, int r);
    static bool inBounds(int c, int r);
    static bool isBlocked(int c, int r);
    static bool isWalkable(int c, int r);
    static Vector2D cellCenter(int c, int r); // world coords
    static void worldToCell(const Vector2D& w, int& c, int& r);

    // Static solid-tile index used as a collision broad-phase
    static void registerSolid(int c, int r, ColliderComponent* col);
    static const std::vector<ColliderComponent*>& solidsAt(int c, int r);

    static int cols() { return s_cols; }
    static int rows() { return s_rows; }
    static int tileW() { return s_tileW; }
    static int tileH() { return s_tileH; }
    static bool ready() { return s_cols > 0 && s_rows > 0; }

private:
    static int s_cols;
    static int s_rows;
    static int s_tileW;
    static int s_tileH;
    static std::vector<uint8_t> s_blocked;
    static std::vector<std::vector<ColliderComponent*>> s_solids;
};

#endif // NAVGRID_H
