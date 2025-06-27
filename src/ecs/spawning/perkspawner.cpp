#include "perkspawner.h"
#include "game.h"
#include "utils.h"
#include "collision.h"

static std::array<std::string, 4> weaponz{"pistol", "uzi", "shotgun", "chainsaw"};

void PerkSpawnerComponent::init()
{
    killsFromLastPerk = 0;
}

void PerkSpawnerComponent::update()
{

    if(scoreData.kills > 0 && scoreData.kills - killsFromLastPerk > perkEveryKills)
    {
        spawnPerk();
        killsFromLastPerk = scoreData.kills;
    }
}

void PerkSpawnerComponent::spawnPerk()
{
    auto& perk = entity->m_manager.addEntity();

    auto width = 32;
    auto height = 32;

    std::string theWeapon = weaponz[rand() % weaponz.size()];

    Vector2D spawnPt = scoreData.lastKillPosition;

    perk.addComponent<TransformComponent>(spawnPt.x,spawnPt.y, width, height);
    perk.addComponent<DecayComponent>(10*TIME_SECOND);
    perk.addComponent<PerkComponent>(theWeapon);
    perk.emplaceComponent<SpriteComponent>().setSrcRect({0,0, 64,64});
    perk.emplaceComponent<ColliderComponent>().onCollision([=,&perk](Entity& target){
        Entity* player = target.m_manager.get("player");
        if(&target == player){
            player->getComponent<WeaponBayComponent>()
                    .autoequip(theWeapon);
            perk.setActive(false);
        }
    });

    perk.addComponent<TextComponent>(theWeapon.substr(0, 1), SDL_Rect({8, 8, 16, 16})).setAbsolute(false);

    perk.addGroup(groupPerks);

}
