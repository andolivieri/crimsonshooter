#include "overlaycomponent.h"
#include "components.h"
#include "utils.h"

#define CG_KILLCOUNT "Kills"
#define CG_HEALTH "Health"

void OverlayComponent::init()
{
    family = &entity->emplaceComponent<RelationshipComponent>();
    auto overalyWidth = 256;
    auto overalyHeight = 100;

    entity->emplaceComponent<SpriteComponent>("assets/overlay.png");
    entity->getComponent<SpriteComponent>()
            .setSrcRect({0,0,overalyWidth,overalyHeight});
    entity->getComponent<TransformComponent>().pos.x = Game::winWidth - overalyWidth - 16;
    entity->getComponent<TransformComponent>().width = overalyWidth;
    entity->getComponent<TransformComponent>().height = overalyHeight;


    auto margin = 16;
    auto lineSpacing = 32;
    auto lineOffset = entity->getComponent<TransformComponent>().pos;
    lineOffset.x+=margin;
    lineOffset.y+=margin;

    auto& killCountLine(entity->m_manager.addEntity());
    killCountLine.addComponent<TransformComponent>();
    killCountLine.getComponent<TransformComponent>().pos.x = lineOffset.x;
    killCountLine.getComponent<TransformComponent>().pos.y = lineOffset.y;
    killCountLine.getComponent<TransformComponent>().width = 100;
    killCountLine.getComponent<TransformComponent>().height = 20;
    killCountLine.addComponent<TextComponent>(CG_KILLCOUNT);
    killCountLine.addGroup(groupOverlay);

    lineOffset.y = killCountLine.getComponent<TransformComponent>().pos.y + lineSpacing;

    auto& healthLine(entity->m_manager.addEntity());
    healthLine.addComponent<TransformComponent>();
    healthLine.getComponent<TransformComponent>().pos.x =  lineOffset.x;
    healthLine.getComponent<TransformComponent>().pos.y =  lineOffset.y;
    healthLine.getComponent<TransformComponent>().width = 100;
    healthLine.getComponent<TransformComponent>().height = 20;
    healthLine.addComponent<TextComponent>("Health");
    healthLine.addGroup(groupOverlay);

    family->addChildren(&killCountLine, "killcount");
    family->addChildren(&healthLine, "healthbar");


    entity->addGroup(groupOverlay);
}

void OverlayComponent::update()
{
    auto& killTxt = family->getChild("killcount")->getComponent<TextComponent>();
    killTxt.txt = strRightPad(CG_KILLCOUNT, 8) + std::to_string(score.kills);


    auto& healthTxt = family->getChild("healthbar")->getComponent<TextComponent>();
    auto h = player.getComponent<DamageModelComponent>().health;
    healthTxt.txt = strRightPad(CG_HEALTH, 8) + std::to_string(h > 0 ? h : 0);

}
