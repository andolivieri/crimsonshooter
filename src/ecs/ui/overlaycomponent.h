#ifndef OVERLAYCOMPONENT_H
#define OVERLAYCOMPONENT_H

#include <time.h>
#include <SDL.h>
#include "ecs/ecs.h"
#include "ecs/base/relationshipcomponent.h"
#include "ecs/gameplay/scorecollector.h"

class OverlayComponent : public Component
{
public:

    OverlayComponent(ScoreData& s, Entity& thePlayer): score(s), player(thePlayer){}

    void init() override;
    void update() override;
    void draw() override;


private:
    Entity& player;
    ScoreData& score;
    RelationshipComponent* family;


};
#endif // OVERLAYCOMPONENT_H
