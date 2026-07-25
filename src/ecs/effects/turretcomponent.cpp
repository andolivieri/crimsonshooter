#include "turretcomponent.h"
#include "engine/game.h"
#include "ecs/components.h"
#include "ecs/weapons/weaponbaycomponent.h"
#include "helpers/math2d.h"
#include "helpers/utils.h"

void TurretComponent::init()
{
    m_transform = &entity->getComponent<TransformComponent>();

    entity->emplaceComponent<SpriteComponent>(m_cfg.baseSprite).setSrcRect({0, 0, 64, 64});
    entity->emplaceComponent<RelationshipComponent>();

    // Apply the optional per-turret weapon-param overrides on top of the preset.
    Config cfg = m_cfg;
    auto tweak = [cfg](WeaponData& wp) {
        if(cfg.overrideRate > 0)     wp.rate = static_cast<uint32_t>(cfg.overrideRate);
        if(cfg.overrideDamage > 0)   wp.projectileDamage = cfg.overrideDamage;
        if(cfg.overrideRange > 0)    wp.range = cfg.overrideRange;
        if(cfg.overrideMagazine > 0) wp.magazine = cfg.overrideMagazine;
    };

    m_bay = &entity->addComponent<WeaponBayComponent>();
    m_bay->setAttachPoint(m_cfg.attachPoint, 0).equip(m_cfg.weaponId, 0, tweak);

    // The weapon is a separate entity; make it a child so it dies with the body.
    if(Entity* weapon = m_bay->weaponEntity(0))
        entity->getComponent<RelationshipComponent>().addChildren(weapon, "turret_weapon");

    // Fixed lifetime: fade out over the last second, then self-destruct.
    const int lifeMs = static_cast<int>(m_cfg.duration * ONE_SECOND);
    entity->addComponent<DecayComponent>(lifeMs, lifeMs - ONE_SECOND);

    entity->addGroup(groupTurrets);
}

void TurretComponent::update()
{
    // Find the closest live enemy in range.
    Entity* closest = nullptr;
    double closestDist = m_cfg.engageRange;
    const Vector2D myCenter = m_transform->center();

    for(auto enemy : entity->m_manager.getGroup(groupEnemies))
    {
        if(!enemy->hasComponent<DamageModelComponent>())
            continue;
        if(enemy->getComponent<DamageModelComponent>().isDead())
            continue;

        const double dist = Math2D::distanceBetweenPoints(
            myCenter, enemy->getComponent<TransformComponent>().center());
        if(dist <= closestDist)
        {
            closestDist = dist;
            closest = enemy;
        }
    }

    if(closest)
    {
        m_transform->rotation = Math2D::angleBetweenPoints(
            myCenter, closest->getComponent<TransformComponent>().center());
        m_bay->triggerPull(0);
    }
    else
    {
        m_bay->triggerRelease(0);
    }
}
