#include "triggerzonecomponent.h"

#include "engine/actionregistry.h"
#include "ecs/base/collidercomponent.h"
#include "helpers/collision.h"

void TriggerZoneComponent::update()
{
    if (m_oneShot && m_fired)
    {
        return;
    }

    Entity* player = entity->m_manager.get("player");
    if (!player || !player->hasComponent<ColliderComponent>())
    {
        return;
    }

    const SDL_Rect& playerRect = player->getComponent<ColliderComponent>().collider;

    const bool overlapping = Collision::AABB(playerRect, m_zone);

    if (!overlapping)
    {
        m_fired = false;
        return;
    }

    if (m_fired)
    {
        return;
    }
    m_fired = true;

    if (m_actions)
    {
        m_actions->run(m_action, m_param);
    }
}
