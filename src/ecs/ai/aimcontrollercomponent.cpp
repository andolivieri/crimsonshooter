#include "aimcontrollercomponent.h"
#include <algorithm>
#include <cmath>
#include "engine/game.h"
#include "ecs/base/transformcomponent.h"
#include "ecs/gameplay/damagemodel.h"
#include "helpers/math2d.h"

namespace {

constexpr float MAX_STEP_SECONDS = 0.1f;

bool isLiveTarget(Entity* e)
{
    return e->active()
        && e->hasComponent<TransformComponent>()
        && e->hasComponent<DamageModelComponent>()
        && !e->getComponent<DamageModelComponent>().isDead();
}

} // namespace

AimControllerComponent::Config::Config() : targetGroup(groupEnemies) {}

void AimControllerComponent::init()
{
    m_transform = &entity->getComponent<TransformComponent>();
}


Entity* AimControllerComponent::target() const
{
    if(m_targetTag.empty())
        return nullptr;
    return entity->m_manager.get(m_targetTag);
}

bool AimControllerComponent::targetStillValid() const
{
    Entity* t = target();
    if(!t)
        return false;

    if(!isLiveTarget(t))
        return false;

    if(m_cfg.disengageRange > 0)
    {
        const double dist = Math2D::distanceBetweenPoints(
            m_transform->center(), t->getComponent<TransformComponent>().center());
        if(dist > m_cfg.disengageRange)
            return false;
    }

    return true;
}

std::string AimControllerComponent::acquireTarget() const
{
    std::string closestTag;
    double closestDist = m_cfg.engageRange;
    const Vector2D myCenter = m_transform->center();

    for(auto candidate : entity->m_manager.getGroup(m_cfg.targetGroup))
    {
        if(candidate->tag.empty() || !isLiveTarget(candidate))
            continue;

        const double dist = Math2D::distanceBetweenPoints(
            myCenter, candidate->getComponent<TransformComponent>().center());
        if(dist <= closestDist)
        {
            closestDist = dist;
            closestTag = candidate->tag;
        }
    }

    return closestTag;
}

void AimControllerComponent::update()
{
    // a locked target is only re-picked once it dies, despawns or (optionally)
    // walks out of disengageRange.
    if(!targetStillValid())
        m_targetTag = acquireTarget();

    Entity* t = target();
    if(!t)
    {
        m_aimError = 0;
        return; // nothing to track: hold the current heading
    }

    const double desired = Math2D::angleBetweenPoints(
                               m_transform->center(),
                               t->getComponent<TransformComponent>().center())
                           + m_cfg.aimErrorDeg;

    // Game::deltaTime keeps this in sync with the game-speed multiplier to come.
    const float dt = std::min(Game::deltaTime, MAX_STEP_SECONDS);
    m_transform->rotation = Math2D::stepAngle(m_transform->rotation, desired,
                                              m_cfg.rotationSpeed * dt);
    m_aimError = Math2D::angleDelta(m_transform->rotation, desired);
}
