#ifndef RELATIONSHIPCOMPONENT_H
#define RELATIONSHIPCOMPONENT_H

#include <time.h>
#include <set>
#include <SDL.h>
#include "ecs.h"

class RelationshipComponent : public Component
{
public:

    RelationshipComponent(){}

    void init() override
    {

    }
    void update() override
    {

        if(parent != nullptr)
           entity->setActive(parent->active());

        for(auto child : children)
            child->setActive(entity->active());

    }


    RelationshipComponent& addParent(Entity* p)
    {
        parent = p;
        return *this;
    }

    RelationshipComponent& addChildren(Entity* child)
    {
        if(!child->hasComponent<RelationshipComponent>())
            child->addComponent<RelationshipComponent>();
        auto& rel = child->getComponent<RelationshipComponent>();
        rel.addParent(entity);
        children.insert(child);

        return *this;
    }

private:
    std::set<Entity*> children;
    Entity* parent{nullptr};

};
#endif // RELATIONSHIPCOMPONENT_H
