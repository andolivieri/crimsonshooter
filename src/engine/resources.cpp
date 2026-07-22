#include "engine/resources.h"

#include <iostream>

const Resource* Resources::find(const std::string& path)
{
    const auto& table = embeddedAssets();
    auto it = table.find(path);
    return it == table.end() ? nullptr : &it->second;
}

SDL_RWops* Resources::rwops(const std::string& path)
{
    const Resource* res = find(path);
    if (!res)
    {
        std::cerr << "Resources: no embedded asset for '" << path << "'" << std::endl;
        return nullptr;
    }

    return SDL_RWFromConstMem(res->data, static_cast<int>(res->size));
}

std::string Resources::readString(const std::string& path)
{
    const Resource* res = find(path);
    if (!res)
    {
        std::cerr << "Resources: no embedded asset for '" << path << "'" << std::endl;
        return {};
    }

    return std::string(reinterpret_cast<const char*>(res->data), res->size);
}
