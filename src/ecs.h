#ifndef ECS_H
#define ECS_H


#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component; // Position, AI, Physics, Input, etc
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
// Serve per sapere se una entity ha un dato component
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;


class Component
{
public:
    Entity* entity;

    virtual void init(){}
    virtual void update() {}
    virtual void draw() {}

};

class Entity
{
private:
    bool m_active = true; // false = remove from game
    std::vector<std::unique_ptr<Component>> m_comps;

    ComponentArray m_compsArray;
    ComponentBitSet m_compsBitset;
public:
    void update(){ for(auto& c : m_comps) c->update();}
    void draw(){ for(auto& c : m_comps) c->draw();}
    bool active(){ return m_active; }
    void destroy(){ m_active = false; }

    template <typename T> bool hasComponent()
    {
        return m_compsBitset[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        T* c(new T(std::forward<TArgs>(mArgs)...));

        c->entity = this;
        std::unique_ptr<Component> uPtr(c);
        m_comps.emplace_back(std::move(uPtr));

        m_compsArray[getComponentTypeID<T>()] = c;
        m_compsBitset[getComponentTypeID<T>()] = true;

        c->init();

        return *c;
    }

    template <typename T> T& getComponent() const
    {
        auto ptr(m_compsArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }
};

class EntityManager
{
private:
    std::vector<std::unique_ptr<Entity>> m_entities;

public:
    void update();
    void draw();
    void prune();

    Entity& addEntity();
};

#endif // ECS_H
