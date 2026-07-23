#include "ecs/ai/pathfindingcomponent.h"
#include "engine/navgrid.h"
#include "engine/game.h"
#include "engine/texturemanager.h"
#include "helpers/utils.h"

#include <SDL.h>
#include <queue>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <utility>

namespace {
    constexpr float SQRT2 = 1.41421356f;

    float octile(int dx, int dy)
    {
        dx = std::abs(dx);
        dy = std::abs(dy);
        int lo = dx < dy ? dx : dy;
        return static_cast<float>(dx + dy) + (SQRT2 - 2.0f) * lo;
    }

    // Nearest walkable cell to (c,r), spiralling outwards. Returns true and
    // writes the found cell; false if nothing walkable within maxRadius.
    bool nearestWalkable(int c, int r, int maxRadius, int& outC, int& outR)
    {
        if (NavGrid::isWalkable(c, r)) {
            outC = c;
            outR = r;
            return true;
        }
        for (int rad = 1; rad <= maxRadius; ++rad) {
            for (int dy = -rad; dy <= rad; ++dy) {
                for (int dx = -rad; dx <= rad; ++dx) {
                    // only the ring at this radius
                    if (std::abs(dx) != rad && std::abs(dy) != rad) {
                        continue;
                    }
                    if (NavGrid::isWalkable(c + dx, r + dy)) {
                        outC = c + dx;
                        outR = r + dy;
                        return true;
                    }
                }
            }
        }
        return false;
    }
}

void PathfindingComponent::init()
{
    m_transform = &entity->getComponent<TransformComponent>();
    // Spread recomputes across entities so they don't all run on one frame.
    m_staggerOffset = static_cast<uint32_t>(std::rand()) % (recomputeInterval + 1);
    m_lastRecompute = 0;

    static const SDL_Color palette[] = {
        {255, 255, 255, 255}, // white
        {255, 0,   0,   255}, // red
        {0,   0,   255, 255}, // blue
        {0,   200, 0,   255}, // green
        {160, 32,  240, 255}, // purple
        {255, 255, 0,   255}, // yellow
        {0,   0,   0,   255}, // black
        {0,   127, 255, 255}, // azure
    };
    constexpr int paletteSize = sizeof(palette) / sizeof(palette[0]);
    m_pathColor = palette[std::rand() % paletteSize];
}

void PathfindingComponent::update()
{
    if (!m_hasGoal || !NavGrid::ready()) {
        return;
    }

    uint32_t now = SDL_GetTicks();
    if (now - m_lastRecompute >= recomputeInterval + m_staggerOffset) {
        int gc, gr;
        NavGrid::worldToCell(m_goal, gc, gr);
        int goalCell = NavGrid::inBounds(gc, gr) ? gr * NavGrid::cols() + gc : -1;
        if (goalCell != m_lastGoalCell || !hasPath()) {
            recompute();
        }
        m_lastRecompute = now;
    }

    advanceWaypoint();
}

void PathfindingComponent::recompute()
{
    m_waypoints.clear();
    m_currentWp = 0;

    const int cols = NavGrid::cols();
    const int rows = NavGrid::rows();

    int sc, sr, gc, gr;
    NavGrid::worldToCell(m_transform->center(), sc, sr);
    NavGrid::worldToCell(m_goal, gc, gr);

    m_lastGoalCell = NavGrid::inBounds(gc, gr) ? gr * cols + gc : -1;

    if (!NavGrid::inBounds(sc, sr) || !NavGrid::inBounds(gc, gr)) {
        return; // off-map
    }

    // Snap an unwalkable goal to the closest cell.
    if (!NavGrid::isWalkable(gc, gr)) {
        int wc, wr;
        if (!nearestWalkable(gc, gr, 8, wc, wr)) {
            return;
        }
        gc = wc;
        gr = wr;
    }

    const int start = sr * cols + sc;
    const int goal = gr * cols + gc;

    if (start == goal) {
        return;
    }

    const int n = cols * rows;
    std::vector<float> gScore(n, std::numeric_limits<float>::infinity());
    std::vector<int> cameFrom(n, -1);
    std::vector<uint8_t> closed(n, 0);

    using Node = std::pair<float, int>; // (fScore, cellIndex)
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open;

    gScore[start] = 0.0f;
    open.push({octile(sc - gc, sr - gr), start});

    // distances col/rows
    static const int DC[8] = {1, -1, 0, 0, 1, 1, -1, -1};
    static const int DR[8] = {0, 0, 1, -1, 1, -1, 1, -1};

    // A* + octile heuristic
    bool found = false;
    while (!open.empty()) {
        int cur = open.top().second;
        open.pop();
        if (closed[cur]) {
            continue;
        }
        closed[cur] = 1;
        if (cur == goal) {
            found = true;
            break;
        }

        int cc = cur % cols;
        int cr = cur / cols;

        for (int i = 0; i < 8; ++i) {
            int nc = cc + DC[i];
            int nr = cr + DR[i];
            if (!NavGrid::isWalkable(nc, nr)) {
                continue;
            }
            bool diagonal = DC[i] != 0 && DR[i] != 0;
            if (diagonal) {
                // don't cut corners: both orthogonal neighbours must be open.
                if (NavGrid::isBlocked(cc + DC[i], cr) ||
                    NavGrid::isBlocked(cc, cr + DR[i])) {
                    continue;
                }
            }
            int nIdx = nr * cols + nc;
            if (closed[nIdx]) {
                continue;
            }
            float tentative = gScore[cur] + (diagonal ? SQRT2 : 1.0f);
            if (tentative < gScore[nIdx]) {
                gScore[nIdx] = tentative;
                cameFrom[nIdx] = cur;
                open.push({tentative + octile(nc - gc, nr - gr), nIdx});
            }
        }
    }

    if (!found) {
        return; // unreachable
    }

    // Reconstruct start -> goal
    std::vector<int> cells;
    for (int at = goal; at != -1; at = cameFrom[at]) {
        cells.push_back(at);
    }
    // cells is goal..start; walk it backwards, skipping the start cell.
    for (int i = static_cast<int>(cells.size()) - 2; i >= 0; --i) {
        int idx = cells[i];
        m_waypoints.push_back(NavGrid::cellCenter(idx % cols, idx / cols));
    }
    m_currentWp = 0;
}

void PathfindingComponent::advanceWaypoint()
{
    if (m_waypoints.empty()) {
        return;
    }
    const float reachDist = NavGrid::tileW() * 0.5f;
    Vector2D pos = m_transform->center();
    while (m_currentWp < m_waypoints.size()) {
        float dx = m_waypoints[m_currentWp].x - pos.x;
        float dy = m_waypoints[m_currentWp].y - pos.y;
        if (dx * dx + dy * dy > reachDist * reachDist) {
            break;
        }
        ++m_currentWp;
    }
}

void PathfindingComponent::draw()
{
    if(entity->hasComponent<DamageModelComponent>() &&
        entity->getComponent<DamageModelComponent>().isDead()){
            return;
        }

    if (!Game::debugPaths || !hasPath()) {
        return;
    }

    SDL_Renderer* r = TextureManager::renderer;
    SDL_SetRenderDrawColor(r, m_pathColor.r, m_pathColor.g, m_pathColor.b, m_pathColor.a);

    std::vector<Vector2D> pts;
    pts.reserve(m_waypoints.size() - m_currentWp + 1);
    pts.push_back(m_transform->center());
    for (size_t i = m_currentWp; i < m_waypoints.size(); ++i) {
        pts.push_back(m_waypoints[i]);
    }
    if (pts.size() < 2) {
        return;
    }

    // Collapse collinear runs
    std::vector<Vector2D> corners;
    corners.reserve(pts.size());
    corners.push_back(pts.front());
    for (size_t i = 1; i + 1 < pts.size(); ++i) {
        float ax = pts[i].x - pts[i - 1].x;
        float ay = pts[i].y - pts[i - 1].y;
        float bx = pts[i + 1].x - pts[i].x;
        float by = pts[i + 1].y - pts[i].y;
        float cross = ax * by - ay * bx;
        if (std::fabs(cross) > 0.01f) {
            corners.push_back(pts[i]); // direction change -> real corner
        }
    }
    corners.push_back(pts.back());

    // Draw one line per straight segment, and a circle at every corner
    Vector2D prev = Game::worldToCamera(corners.front());
    for (size_t i = 1; i < corners.size(); ++i) {
        Vector2D p = Game::worldToCamera(corners[i]);
        SDL_RenderDrawLine(r, static_cast<int>(prev.x), static_cast<int>(prev.y),
                              static_cast<int>(p.x), static_cast<int>(p.y));
        drawCircle(r, {static_cast<int>(p.x), static_cast<int>(p.y)}, 3, m_pathColor);
        prev = p;
    }
}
