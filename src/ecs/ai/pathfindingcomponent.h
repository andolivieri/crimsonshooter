#ifndef PATHFINDINGCOMPONENT_H
#define PATHFINDINGCOMPONENT_H

#include <vector>
#include <cstdint>
#include <SDL.h>
#include "ecs/ecs.h"
#include "ecs/base/transformcomponent.h"
#include "helpers/vector2d.h"

// Grid A* pathfinder + path visualizer.
class PathfindingComponent : public Component
{
public:
    PathfindingComponent() = default;

    void init() override;
    void update() override;
    void draw() override;

    // Set the world-space goal to path toward. Cheap; recompute is cadence-gated.
    void setGoal(const Vector2D& worldGoal) { m_goal = worldGoal; m_hasGoal = true; }

    bool hasPath() const { return m_currentWp < m_waypoints.size(); }
    Vector2D nextWaypoint() const { return m_waypoints[m_currentWp]; }

    // Recompute cadence in milliseconds.
    uint32_t recomputeInterval = 500;

private:
    void recompute();
    void advanceWaypoint();

    TransformComponent* m_transform = nullptr;

    Vector2D m_goal;
    bool m_hasGoal = false;

    std::vector<Vector2D> m_waypoints;
    size_t m_currentWp = 0;

    uint32_t m_lastRecompute = 0;
    uint32_t m_staggerOffset = 0;
    int m_lastGoalCell = -1;

    SDL_Color m_pathColor = {255, 255, 255, 255};
};

#endif // PATHFINDINGCOMPONENT_H
