#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H



#include <time.h>
#include <SDL.h>
#include "ecs.h"

class SoundComponent : public Component
{
public:

    SoundComponent(const std::string _sound)
    {

    }

    void init() override;
    void update() override;

    void play(const std::string&);


private:
    int dummyValue;
    time_t startTime;

};
#endif // SOUNDCOMPONENT_H
