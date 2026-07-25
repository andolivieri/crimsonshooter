#include "weaponbaycomponent.h"
#include "projectilecomponent.h"
#include "engine/assetmanager.h"
#include "helpers/weaponfactory.h"
#include "ecs/base/fsmcomponent.h"
#include "weaponcomponent.h"

WeaponBayComponent::WeaponBayComponent() : grenadeCount(5)
{

}

WeaponBayComponent &WeaponBayComponent::setAttachPoint(const Vector2D& a, int slot, bool mirrored)
{
    slots[slot] = a;
    mirror[slot] = mirrored;
    return *this;
}

void WeaponBayComponent::init()
{

    transform = &entity->getComponent<TransformComponent>();


    if(!entity->hasComponent<RelationshipComponent>())
        entity->addComponent<RelationshipComponent>();
    rel = &entity->getComponent<RelationshipComponent>();


}

WeaponBayComponent & WeaponBayComponent::equip(const std::string& weaponId, int slot)
{
    return equip(weaponId, slot, {});
}

WeaponBayComponent & WeaponBayComponent::equip(const std::string& weaponId, int slot, const std::function<void(WeaponData&)>& tweak)
{

    if(weaponId == "grenade"){
        grenadeCount++;
    }else{
        AttachedWeapon a;
        a.weaponId = weaponId;
        a.entity = &WeaponFactory(entity->m_manager).createWeaponEntity(weaponId, mirror[slot], tweak);
        a.attachPoint = slots[slot];
        drop(slot);
        weapons[slot] = a;
    }

    return *this;

}

Entity* WeaponBayComponent::weaponEntity(int slot) const
{
    return weapons[slot].entity;
}

void WeaponBayComponent::triggerPull(int slot)
{
    if(weapons[slot].entity)
        weapons[slot].entity->getComponent<WeaponComponent>().triggerPull();
}

void WeaponBayComponent::triggerRelease(int slot)
{
    if(weapons[slot].entity)
        weapons[slot].entity->getComponent<WeaponComponent>().triggerRelease();
}

void WeaponBayComponent::reload(int slot)
{
    if(weapons[slot].entity)
        weapons[slot].entity->getComponent<WeaponComponent>().reload();
}

WeaponBayComponent &WeaponBayComponent::autoequip(const std::string &n)
{
    if(weapons[0].weaponId == n){
        equip(n, 1);
        equip(n, 0);
    }else{
        drop(1);
        equip(n, 0);
    }
    return *this;

}

void WeaponBayComponent::drop(int slot)
{
    if(weapons[slot].entity)
        weapons[slot].entity->setActive(false);
    weapons[slot] = AttachedWeapon();
}


void WeaponBayComponent::update()
{

    for(auto i=0; i<weapons.size();i++)
    {
        if(weapons[i].entity){

            auto center = transform->center();
            auto attMargin = weapons[i].entity->getComponent<WeaponComponent>().getAttachMargin();
            center.x += weapons[i].attachPoint.x + attMargin.x;
            center.y += weapons[i].attachPoint.y +  attMargin.y;
            Vector2D rotatedCenter = Math2D::rotate_point(transform->center(), transform->rotation, center);
            weapons[i].entity->getComponent<TransformComponent>().centerOn(rotatedCenter);
            weapons[i].entity->getComponent<TransformComponent>().rotation = transform->rotation;
        }
    }

}
