#include "ecs.h"














void EntityManager::update()
{
    for(auto& e : m_entities) e->update();
}

void EntityManager::draw()
{
    for(auto& e : m_entities) e->draw();
}

void EntityManager::prune()
{
    m_entities.erase(
                std::remove_if(
                    std::begin(m_entities),
                    std::end(m_entities),
                    [](const std::unique_ptr<Entity> &e){return !e->active();}
                ),
                std::end(m_entities));
}

Entity &EntityManager::addEntity()
{
    Entity *e = new Entity;
    std::unique_ptr<Entity> uPtr(e);
    m_entities.emplace_back(std::move(uPtr));
    return *e;
}
