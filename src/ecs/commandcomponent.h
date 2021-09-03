#ifndef COMMANDCOMPONENT_H
#define COMMANDCOMPONENT_H

#include <time.h>
#include <deque>
#include <SDL.h>
#include "ecs.h"
#include "transformcomponent.h"

// TODO is this shit really necessary? maybe not...

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

    enum MoveDirection {
        DIRECTION_UP,
        DIRECTION_DOWN,
        DIRECTION_LEFT,
        DIRECTION_RIGHT,
    };

    MoveDirection direction;
    float speed = 2;

    MoveCommand(MoveDirection d):
        direction(d)
    {

    }

    void execute() override
    {

        std::cout << "MoveCommand: " << direction << std::endl;
        auto& transform = entity->getComponent<TransformComponent>();
        switch (direction) {
        case DIRECTION_UP:
            transform.velocity.y = -speed;
            break;
        case DIRECTION_DOWN:
            transform.velocity.y = speed;
            break;
        case DIRECTION_RIGHT:
            transform.velocity.x = speed;
            break;
        case DIRECTION_LEFT:
            transform.velocity.x = -speed;
            break;
        default:
            break;
        }
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
