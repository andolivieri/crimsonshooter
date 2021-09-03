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

        while(!soundQueue.empty())
        {
            auto e = soundQueue.front();
            Mix_PlayChannel(e.channel, e.chunk, e.loops);
            soundQueue.pop_front();
        }

    }

    void play(const std::string& soundId, int loops=0,int channel=1)
    {
        if(soundId.empty())
            return;

        Mix_Chunk* chunk = AssetManager::getSound(soundId);
        assert(chunk);
        soundQueue.push_back({chunk, loops, channel});
    }



private:
    std::deque<SoundEvent> soundQueue;

};
#endif // SOUNDCOMPONENT_H
