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
    static ComponentID typeID = getComponentID();
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

    virtual void init();
    virtual void update();
    virtual void draw();

    virtual ~Component();
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

    template <typename T> bool hasComponent();

    template <typename T, typename...TArgs>
    T& addComponent(TArgs&&... mArgs);
    template <typename T> T& getComponent() const;
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
