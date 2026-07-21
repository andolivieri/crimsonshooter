#ifndef FOEREGISTRY_H
#define FOEREGISTRY_H

#include <string>
#include <SDL.h>

struct FoeAnim
{
    int col;
    int row;
    int frames;
    uint32_t speed;
};

struct FoeKindData
{
    std::string sprite = "assets/foe.png";
    SDL_Rect srcRect{0, 0, 32, 32};

    FoeAnim moving{0, 0, 4, 200};
    FoeAnim dying{4, 0, 4, 150};
    FoeAnim idle{8, 0, 2, 500};
    FoeAnim critical{10, 0, 3, 50};

    int health = 30;
    float speedMin = 0.5f;
    float speedMax = 1.5f;
    int contactDamage = 10;
};


class FoeRegistry
{
public:
    static const FoeKindData& get(const std::string& kind);
};

#endif // FOEREGISTRY_H
