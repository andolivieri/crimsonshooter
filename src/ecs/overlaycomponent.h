#ifndef OVERLAYCOMPONENT_H
#define OVERLAYCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs.h"
#include "relationshipcomponent.h"
#include "scorecollector.h"

class OverlayComponent : public Component
{
public:

    OverlayComponent(ScoreData& s, Entity& thePlayer): score(s), player(thePlayer){}

    void init() override;
    void update() override;
    //void draw() override;


private:
    Entity& player;
    ScoreData& score;
    RelationshipComponent* family;


};
#endif // OVERLAYCOMPONENT_H
