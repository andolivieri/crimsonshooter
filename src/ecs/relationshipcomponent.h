#ifndef RELATIONSHIPCOMPONENT_H
#define RELATIONSHIPCOMPONENT_H

#include <time.h>
#include <map>
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

        // If parent dies, I die
        if(parent != nullptr && !parent->active())
           entity->setActive(false);

        // Propagate death
        for(auto kv : children){
            auto child = kv.second;
            child->setActive(entity->active());
        }

        // Remove from parent
        if(!entity->active() && parent){
            auto& rel = parent->getComponent<RelationshipComponent>();
            rel.removeChildren(mytag);
        }

    }


    RelationshipComponent& addParent(Entity* p)
    {
        parent = p;
        return *this;
    }

    RelationshipComponent& addChildren(Entity* child, const std::string& tag)
    {
        if(!child->hasComponent<RelationshipComponent>())
            child->addComponent<RelationshipComponent>();
        auto& rel = child->getComponent<RelationshipComponent>();
        rel.addParent(entity);
        children[tag] = child;

        return *this;
    }

    Entity* getChildren(const std::string& tag)
    {
        return children[tag];
    }


    RelationshipComponent& removeChildren(const std::string& tag)
    {
        children.erase(tag);
        return *this;
    }

private:
    const std::string mytag;
    std::map<std::string, Entity*> children;
    Entity* parent{nullptr};

};
#endif // RELATIONSHIPCOMPONENT_H
