#include "overlaycomponent.h"
#include "components.h"

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


    auto& killCountLine(entity->m_manager.addEntity());
    auto overlaypos = entity->getComponent<TransformComponent>().pos;
    killCountLine.addComponent<TransformComponent>();
    killCountLine.getComponent<TransformComponent>().pos.x = overlaypos.x + 16;
    killCountLine.getComponent<TransformComponent>().pos.y = overlaypos.y + 16;
    killCountLine.getComponent<TransformComponent>().width = 100;
    killCountLine.getComponent<TransformComponent>().height = 20;
    killCountLine.addComponent<TextComponent>("Kills");
    killCountLine.addGroup(groupOverlay);
    family->addChildren(&killCountLine, "killcount");


    entity->addGroup(groupOverlay);
}

void OverlayComponent::update()
{
    auto& killTxt = family->getChild("killcount")->getComponent<TextComponent>();
    killTxt.txt = "Kills " + std::to_string(score.kills);

}
