#include "collisiongridcomponent.h"
#include "engine/game.h"

CollisionGridComponent& CollisionGridComponent::get(EntityManager& manager)
{
    Entity* gridEntity = manager.get("collisiongrid");
    if(!gridEntity){
        gridEntity = &manager.addEntity("collisiongrid");
        gridEntity->addComponent<CollisionGridComponent>();
    }
    return gridEntity->getComponent<CollisionGridComponent>();
}

std::vector<Entity*> CollisionGridComponent::query(int x, int y, int w, int h)
{
    if(m_builtTick != Game::tick){
        m_grid.rebuild(entity->m_manager.getGroup(groupColliders));
        m_builtTick = Game::tick;
    }
    return m_grid.query(x, y, w, h);
}
