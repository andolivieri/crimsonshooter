#ifndef TRIGGERZONECOMPONENT_H
#define TRIGGERZONECOMPONENT_H

#include <string>
#include <SDL.h>
#include "ecs/ecs.h"

class ActionRegistry;

//  action's behavior lives in the registry
class TriggerZoneComponent : public Component
{
public:
    TriggerZoneComponent(const SDL_Rect& zone,
                         std::string action,
                         std::string param,
                         const ActionRegistry& actions,
                         bool oneShot = true)
        : m_zone(zone),
          m_action(std::move(action)),
          m_param(std::move(param)),
          m_actions(&actions),
          m_oneShot(oneShot)
    {
    }

    void update() override;

private:
    SDL_Rect m_zone;
    std::string m_action;
    std::string m_param;
    const ActionRegistry* m_actions;
    bool m_oneShot;
    bool m_fired = false;
};

#endif // TRIGGERZONECOMPONENT_H
