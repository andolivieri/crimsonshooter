#include <SDL.h>
#include "overlaycomponent.h"
#include "ecs/components.h"
#include "utils.h"
#include <game.h>


#define HEALTHBARWIDTH 80
#define HEALTHBARHEIGHT 12
#define STAMINABARWIDTH 80
#define STAMINABARHEIGHT 12
void OverlayComponent::init()
{
    auto iconSize = 32;
    auto iconTextDistance = 5;
    family = &entity->emplaceComponent<RelationshipComponent>();
    auto overalyWidth = 420;
    auto overalyHeight = 50;

    entity->emplaceComponent<TransformComponent>().pos.x = Game::winWidth - overalyWidth - 16;
    entity->getComponent<TransformComponent>().width = overalyWidth;
    entity->getComponent<TransformComponent>().height = overalyHeight;


    auto margin = 12;
    auto itemSpacing = 80;
    auto centerY = entity->getComponent<TransformComponent>().pos.y + (overalyHeight / 2) - 16;
    auto baseOffset = entity->getComponent<TransformComponent>().pos;
    baseOffset.x += margin;
    baseOffset.y = centerY;

    auto& killCountLine(entity->m_manager.addEntity());
    killCountLine.addComponent<TransformComponent>(baseOffset.x, baseOffset.y, iconSize, iconSize);
    killCountLine.addComponent<SpriteComponent>("assets/skull.png");
    killCountLine.getComponent<SpriteComponent>().setAbsolute(true);
    killCountLine.addGroup(groupOverlay);

    auto& killCountText(entity->m_manager.addEntity());
    killCountText.addComponent<TransformComponent>(baseOffset.x + iconSize + iconTextDistance, baseOffset.y + 8, 40, 16);
    killCountText.addComponent<TextComponent>();
    killCountText.addGroup(groupOverlay);

    baseOffset.x += itemSpacing;

    auto& grenadeCountLine(entity->m_manager.addEntity());
    grenadeCountLine.addComponent<TransformComponent>(baseOffset.x, baseOffset.y, iconSize, iconSize);
    grenadeCountLine.addComponent<SpriteComponent>("assets/grenade.png");
    grenadeCountLine.getComponent<SpriteComponent>().setAbsolute(true);
    grenadeCountLine.addGroup(groupOverlay);

    auto& grenadeCountText(entity->m_manager.addEntity());
    grenadeCountText.addComponent<TransformComponent>(baseOffset.x + iconSize + iconTextDistance, baseOffset.y + 8, 40, 16);
    grenadeCountText.addComponent<TextComponent>();
    grenadeCountText.addGroup(groupOverlay);

    baseOffset.x += itemSpacing;

    auto& healthLine = entity->m_manager.addEntity();
    healthLine.addComponent<TransformComponent>(baseOffset.x, baseOffset.y, iconSize, iconSize);
    healthLine.addComponent<SpriteComponent>("assets/life.png");
    healthLine.getComponent<SpriteComponent>().setAbsolute(true);
    healthLine.addGroup(groupOverlay);

    auto& healthBar(entity->m_manager.addEntity());
    healthBar.addComponent<TransformComponent>(baseOffset.x + iconSize + iconTextDistance, centerY + 12, HEALTHBARWIDTH, HEALTHBARHEIGHT );
    healthBar.emplaceComponent<DumbComponent>().onDraw([&](){
        auto& tr = healthBar.getComponent<TransformComponent>();
        SDL_SetRenderDrawColor(TextureManager::renderer, 136,8,8,255);
        auto& dmodel = player.getComponent<DamageModelComponent>();
        auto width = (HEALTHBARWIDTH * dmodel.health) / dmodel.startHealth;
        SDL_Rect r{tr.pos.x, tr.pos.y, width, tr.height};
        SDL_RenderFillRect(TextureManager::renderer, &r);
    });
    healthBar.addGroup(groupOverlay);

    baseOffset.x += 32 + HEALTHBARWIDTH + iconTextDistance;

    auto& staminaLine(entity->m_manager.addEntity());
    
    staminaLine.addComponent<TransformComponent>(baseOffset.x, baseOffset.y, iconSize, iconSize);
    staminaLine.addComponent<SpriteComponent>("assets/stamina.png");
    staminaLine.getComponent<SpriteComponent>().setAbsolute(true);
    staminaLine.addGroup(groupOverlay);

    auto& staminaBar(entity->m_manager.addEntity());
    
    staminaBar.addComponent<TransformComponent>(baseOffset.x + iconSize + iconTextDistance, centerY + 12, STAMINABARWIDTH, STAMINABARHEIGHT );
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
    family->addChildren(&killCountText, "killcounttext");
    family->addChildren(&grenadeCountLine, "grenadecount");
    family->addChildren(&grenadeCountText, "grenadecounttext");
    family->addChildren(&healthLine, "healthbar");
    family->addChildren(&healthBar, "health");
    family->addChildren(&staminaLine, "staminabar");
    family->addChildren(&staminaBar, "stamina");


    entity->addGroup(groupOverlayBg);
}

void OverlayComponent::update()
{
    auto& killTxt = family->getChild("killcounttext")->getComponent<TextComponent>();
    killTxt.txt = strLeftPad(std::to_string(score.kills), 3, ' ' );

    auto& grenadeTxt = family->getChild("grenadecounttext")->getComponent<TextComponent>();
    auto& wb = player.getComponent<WeaponBayComponent>();
    grenadeTxt.txt = strLeftPad(std::to_string(wb.grenadeCount), 3, ' ' );
    
}


void OverlayComponent::draw()
{


}
