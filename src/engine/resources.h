#ifndef RESOURCES_H
#define RESOURCES_H

#include "SDL.h"
#include <cstddef>
#include <string>
#include <unordered_map>

// represents a single embedded asset
struct Resource
{
    const unsigned char* data;
    std::size_t size;
};

// Defined in the generated translation unit
const std::unordered_map<std::string, Resource>& embeddedAssets();


class Resources
{
public:
    static const Resource* find(const std::string& path);
    static SDL_RWops* rwops(const std::string& path);
    static std::string readString(const std::string& path);
};

#endif // RESOURCES_H
