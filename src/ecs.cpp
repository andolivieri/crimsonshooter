#include "ecs.h"










template<typename T>
bool Entity::hasComponent()
{
    return m_compsBitset[getComponentTypeID<T>()];
}

template<typename T, typename... TArgs>
T& Entity::addComponent(TArgs&&...mArgs)
{
    T* c(new T(std::forward<TArgs>)(mArgs)...);

    c->entity = this;
    std::unique_ptr<Component> uPtr(c);
    m_comps.emplace_back(uPtr);

    m_compsArray[getComponentTypeID<T>()] = c;
    m_compsBitset[getComponentTypeID<T>()] = true;

    c->init();
}

template<typename T>
T& Entity::getComponent() const
{
    auto ptr(m_compsArray[getComponentTypeID<T>()]);
    return *static_cast<T*>(ptr);
}

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
