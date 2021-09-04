#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H


#include <deque>
#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "assetmanager.h"
#include "assert.h"


struct SoundEvent
{
    Mix_Chunk* chunk;
    int loops;
    int channel;
};

class SoundComponent : public Component
{
public:

    SoundComponent()
    {
    }

    void init() override
    {

    }


    void update() override
    {

    }

    void play(const std::string& soundId, int loops=0,int channel=1)
    {

        if(soundId.empty())
            return;

        Mix_Chunk* chunk = AssetManager::getSound(soundId);
        assert(chunk);
        SoundEvent e = {chunk, loops, channel};
        Mix_PlayChannel(e.channel, e.chunk, e.loops);
    }



private:

};
#endif // SOUNDCOMPONENT_H
