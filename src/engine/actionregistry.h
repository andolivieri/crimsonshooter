#ifndef ACTIONREGISTRY_H
#define ACTIONREGISTRY_H

#include <functional>
#include <string>
#include <unordered_map>


class ActionRegistry
{
public:
    using Action = std::function<void(const std::string& param)>;

    void reg(const std::string& name, Action action)
    {
        m_actions[name] = std::move(action);
    }

    // Runs the named action with param. Falls back to the global registry when
    // this instance doesn't define it. Returns false if nobody handled it.
    bool run(const std::string& name, const std::string& param) const
    {
        auto it = m_actions.find(name);
        if (it != m_actions.end())
        {
            it->second(param);
            return true;
        }
        if (this != &global())
        {
            return global().run(name, param);
        }
        return false;
    }

    // Process-wide registry for actions shared by every level.
    static ActionRegistry& global()
    {
        static ActionRegistry instance;
        return instance;
    }

private:
    std::unordered_map<std::string, Action> m_actions;
};

#endif // ACTIONREGISTRY_H
