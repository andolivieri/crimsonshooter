#include "engine/navgrid.h"

int NavGrid::s_cols = 0;
int NavGrid::s_rows = 0;
int NavGrid::s_tileW = 0;
int NavGrid::s_tileH = 0;
std::vector<uint8_t> NavGrid::s_blocked;

void NavGrid::reset(int cols, int rows, int tileW, int tileH)
{
    s_cols = cols;
    s_rows = rows;
    s_tileW = tileW;
    s_tileH = tileH;
    s_blocked.assign(static_cast<size_t>(cols) * rows, 0);
}

void NavGrid::setBlocked(int c, int r)
{
    if (!inBounds(c, r)) {
        return;
    }
    s_blocked[static_cast<size_t>(r) * s_cols + c] = 1;
}

bool NavGrid::inBounds(int c, int r)
{
    return c >= 0 && r >= 0 && c < s_cols && r < s_rows;
}

bool NavGrid::isBlocked(int c, int r)
{
    if (!inBounds(c, r)) {
        return true; // out of bounds counts as blocked
    }
    return s_blocked[static_cast<size_t>(r) * s_cols + c] != 0;
}

bool NavGrid::isWalkable(int c, int r)
{
    return inBounds(c, r) && !isBlocked(c, r);
}

Vector2D NavGrid::cellCenter(int c, int r)
{
    return Vector2D{static_cast<float>(c * s_tileW + s_tileW / 2),
                    static_cast<float>(r * s_tileH + s_tileH / 2)};
}

void NavGrid::worldToCell(const Vector2D& w, int& c, int& r)
{
    c = s_tileW > 0 ? static_cast<int>(w.x) / s_tileW : 0;
    r = s_tileH > 0 ? static_cast<int>(w.y) / s_tileH : 0;
}
