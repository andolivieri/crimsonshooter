#include "turretcomponent.h"
#include "engine/game.h"
#include "ecs/components.h"
#include "ecs/weapons/weaponbaycomponent.h"
#include "helpers/math2d.h"
#include "helpers/utils.h"

void TurretComponent::init()
{
    m_transform = &entity->getComponent<TransformComponent>();

    entity->emplaceComponent<RelationshipComponent>();

    auto& tripod = entity->m_manager.addEntity();
    tripod.addComponent<TransformComponent>(m_transform->pos.x, m_transform->pos.y,
                                           m_transform->width, m_transform->height);
    m_bodyTransform = &tripod.getComponent<TransformComponent>();
    tripod.emplaceComponent<SpriteComponent>(m_cfg.baseSprite).setSrcRect({0, 0, 64, 64});
    tripod.addGroup(groupTurrets);
    entity->getComponent<RelationshipComponent>().addChildren(&tripod, "turret_body");

    // added before the bay so the weapon follows the new heading in the same frame.
    AimControllerComponent::Config aimCfg;
    aimCfg.rotationSpeed  = m_cfg.rotationSpeed;
    aimCfg.engageRange    = m_cfg.engageRange;
    aimCfg.disengageRange = m_cfg.disengageRange;
    m_aim = &entity->addComponent<AimControllerComponent>(aimCfg);

    // optional per-turret weapon-param overrides on top of the preset.
    Config cfg = m_cfg;
    auto tweak = [cfg](WeaponData& wp) {
        if(cfg.overrideRate > 0)     wp.rate = static_cast<uint32_t>(cfg.overrideRate);
        if(cfg.overrideDamage > 0)   wp.projectileDamage = cfg.overrideDamage;
        if(cfg.overrideRange > 0)    wp.range = cfg.overrideRange;
        if(cfg.overrideMagazine > 0) wp.magazine = cfg.overrideMagazine;
    };

    m_bay = &entity->addComponent<WeaponBayComponent>();
    m_bay->setAttachPoint(m_cfg.attachPoint, 0).equip(m_cfg.weaponId, 0, tweak);

    if(Entity* weapon = m_bay->weaponEntity(0))
        entity->getComponent<RelationshipComponent>().addChildren(weapon, "turret_weapon");

    const int lifeMs = static_cast<int>(m_cfg.duration * ONE_SECOND);
    entity->addComponent<DecayComponent>(lifeMs, lifeMs - ONE_SECOND);

    entity->addGroup(groupTurrets);
}

void TurretComponent::update()
{
    m_bodyTransform->centerOn(m_transform->center());

    // AimControllerComponent owns target selection and the rotation limit; the
    // turret just holds the trigger down while it has something to track, so it
    // keeps hosing whatever it sweeps across on the way there.
    if(m_aim->hasTarget())
        m_bay->triggerPull(0);
    else
        m_bay->triggerRelease(0);
}
