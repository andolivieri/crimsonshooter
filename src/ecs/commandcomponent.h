#ifndef COMMANDCOMPONENT_H
#define COMMANDCOMPONENT_H

#include <time.h>
#include <deque>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"

// ALWAYS PLACE THIS AS THE LAST COMPONENT
// OR Command.entity might BE DESTROYED MEANWHILE

// For "high" level commands like: move, attack-move, etc
class CommandBase
{
public:
    Entity* entity;

    virtual void execute() {}
};

class MoveCommand : public CommandBase
{
public:

    float velocityX;
    float velocityY;

    MoveCommand(){}

    void execute() override
    {
        auto transform = entity->getComponent<TransformComponent>();
        //transform.velocity.x;
    }
};

class CommandComponent : public Component
{
public:

    CommandComponent(){}

    void update() override
    {
        while(!queuedCommands.empty())
        {
            auto& c = queuedCommands.front();
            c->execute();
            queuedCommands.pop_front();
        }
    }


    template <typename T, typename... TArgs>
    T& addCommand(TArgs&&... mArgs)
    {
        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this->entity;
        std::unique_ptr<CommandBase> uPtr(c);
        queuedCommands.push_back(std::move(uPtr));

        return *c;
    }


private:
    std::deque<std::unique_ptr<CommandBase>> queuedCommands;


};
#endif // COMMANDCOMPONENT_H
