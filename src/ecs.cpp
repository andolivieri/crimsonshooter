#include "ecs.h"

bool Entity::hasGroup(Group g)
{
    return m_groupBitset[g];
}

void Entity::addGroup(Group g)
{
    m_groupBitset[g] = true;
    m_manager.addToGroup(this, g);
}

void Entity::delGroup(Group g)
{
    m_groupBitset[g] = false;
}

void EntityManager::update()
{
    for(auto& e : m_entities) e->update();
}

void EntityManager::draw()
{
    for(auto& e : m_entities) e->draw();
}

void EntityManager::refresh()
{

    for(auto i(0u); i < maxGroups; i++)
    {
        auto& v(m_groupedEntities[i]);
        v.erase(
                std::remove_if(
                    std::begin(v),
                    std::end(v),
                    [i](Entity* e){return !e->active() || !e->hasGroup(i);}
                ),
                std::end(v));
    }

    m_entities.erase(
                std::remove_if(
                    std::begin(m_entities),
                    std::end(m_entities),
                    [](const std::unique_ptr<Entity> &e){return !e->active();}
                ),
                std::end(m_entities));
}

void EntityManager::addToGroup(Entity *e, Group g)
{
    m_groupedEntities[g].emplace_back(e);
}

std::vector<Entity *> &EntityManager::getGroup(Group g)
{
    return m_groupedEntities[g];
}

Entity& EntityManager::addEntity()
{
    Entity *e = new Entity(*this);
    std::unique_ptr<Entity> uPtr(e);
    m_entities.emplace_back(std::move(uPtr));
    return *e;
}


