#include "overlaycomponent.h"
#include "components.h"
#include "utils.h"

#define CG_KILLCOUNT "Kills"
#define CG_HEALTH "Health"
#include <SDL.h>

#define HEALTHBARWIDTH 150
#define HEALTHBARHEIGHT 15
#define STAMINABARWIDTH 150
#define STAMINABARHEIGHT 15
void OverlayComponent::init()
{
    family = &entity->emplaceComponent<RelationshipComponent>();
    auto overalyWidth = 256;
    auto overalyHeight = 100;

    entity->emplaceComponent<SpriteComponent>("assets/overlay.png");
    entity->getComponent<SpriteComponent>()
            .setSrcRect({0,0,overalyWidth,overalyHeight})
            .setAbsolute(true);
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
    healthLine.getComponent<TransformComponent>().pos =  lineOffset;
    healthLine.getComponent<TransformComponent>().width = 100;
    healthLine.getComponent<TransformComponent>().height = 20;
    healthLine.addComponent<TextComponent>("Health");
    healthLine.addGroup(groupOverlay);


    lineOffset.y = healthLine.getComponent<TransformComponent>().pos.y + lineSpacing;

    auto& healthBar(entity->m_manager.addEntity());
    healthBar.addComponent<TransformComponent>();
    healthBar.getComponent<TransformComponent>().pos = lineOffset;
    healthBar.getComponent<TransformComponent>().width = HEALTHBARWIDTH;
    healthBar.getComponent<TransformComponent>().height = HEALTHBARHEIGHT;
    healthBar.addComponent<DumbComponent>();
    healthBar.getComponent<DumbComponent>().onDraw([&](){

        auto& tr = healthBar.getComponent<TransformComponent>();

        SDL_SetRenderDrawColor(TextureManager::renderer, 136,8,8,1);
        auto& dmodel = player.getComponent<DamageModelComponent>();
        auto width = (HEALTHBARWIDTH * dmodel.health) / dmodel.startHealth;
        SDL_Rect r{tr.pos.x, tr.pos.y, width, tr.height};
        SDL_RenderFillRect(TextureManager::renderer, &r);
    });
    healthBar.addGroup(groupOverlay);

    lineOffset.y = healthBar.getComponent<TransformComponent>().pos.y + lineSpacing;

    auto& staminaLine(entity->m_manager.addEntity());
    staminaLine.addComponent<TransformComponent>();
    staminaLine.getComponent<TransformComponent>().pos = lineOffset;
    staminaLine.getComponent<TransformComponent>().width = 100;
    staminaLine.getComponent<TransformComponent>().height = 20;
    staminaLine.addComponent<TextComponent>("Stamina");
    staminaLine.addGroup(groupOverlay);

    lineOffset.y = staminaLine.getComponent<TransformComponent>().pos.y + lineSpacing;

    auto& staminaBar(entity->m_manager.addEntity());
    staminaBar.addComponent<TransformComponent>();
    staminaBar.getComponent<TransformComponent>().pos = lineOffset;
    staminaBar.getComponent<TransformComponent>().width = STAMINABARWIDTH;
    staminaBar.getComponent<TransformComponent>().height = STAMINABARHEIGHT;
    staminaBar.addComponent<DumbComponent>();
    staminaBar.getComponent<DumbComponent>().onDraw([&](){
        auto& tr = staminaBar.getComponent<TransformComponent>();
        
        if(player.hasComponent<StaminaComponent>()) {
            SDL_SetRenderDrawColor(TextureManager::renderer, 0, 100, 200, 255);
            auto& stamina = player.getComponent<StaminaComponent>();
            auto width = (STAMINABARWIDTH * stamina.getCurrentStamina()) / stamina.getMaxStamina();
            SDL_Rect r{tr.pos.x, tr.pos.y, static_cast<int>(width), tr.height};
            SDL_RenderFillRect(TextureManager::renderer, &r);
        }
    });
    staminaBar.addGroup(groupOverlay);

    family->addChildren(&killCountLine, "killcount");
    family->addChildren(&healthLine, "healthbar");
    family->addChildren(&healthBar, "health");
    family->addChildren(&staminaLine, "staminabar");
    family->addChildren(&staminaBar, "stamina");


    entity->addGroup(groupOverlayBg);
}

void OverlayComponent::update()
{
    auto& killTxt = family->getChild("killcount")->getComponent<TextComponent>();
    killTxt.txt = strRightPad(CG_KILLCOUNT, 8) + std::to_string(score.kills);

    auto& healthTxt = family->getChild("healthbar")->getComponent<TextComponent>();
    auto h = player.getComponent<DamageModelComponent>().health;
    healthTxt.txt = strRightPad(CG_HEALTH, 8) + std::to_string(h > 0 ? h : 0);

    if(player.hasComponent<StaminaComponent>()) {
        auto& staminaTxt = family->getChild("staminabar")->getComponent<TextComponent>();
        auto stamina = static_cast<int>(player.getComponent<StaminaComponent>().getCurrentStamina());
        staminaTxt.txt = strRightPad("Energy", 8) + std::to_string(stamina > 0 ? stamina : 0);
    }
}


void OverlayComponent::draw()
{


}
