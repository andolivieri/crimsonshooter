#include "weaponbaycomponent.h"
#include "projectilecomponent.h"
#include "tilecomponent.h"
#include "assetmanager.h"
#include "weaponfactory.h"
#include "fsmcomponent.h"

WeaponBayComponent::WeaponBayComponent()
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
    input = &entity->getComponent<InputComponent>();


    if(!entity->hasComponent<RelationshipComponent>())
        entity->addComponent<RelationshipComponent>();
    rel = &entity->getComponent<RelationshipComponent>();


}

WeaponBayComponent & WeaponBayComponent::equip(const std::string& weaponId, int slot)
{

    AttachedWeapon a;
    a.weaponId = weaponId;
    a.entity = &WeaponFactory(entity->m_manager).createWeaponEntity(weaponId, mirror[slot]);
    a.attachPoint = slots[slot];
    drop(slot);
    weapons[slot] = a;
    return *this;

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
            center.x += weapons[i].attachPoint.x;
            center.y += weapons[i].attachPoint.y;
            Vector2D rotatedCenter = Math2D::rotate_point(transform->center(), transform->rotation, center);
            weapons[i].entity->getComponent<TransformComponent>().centerOn(rotatedCenter);
            weapons[i].entity->getComponent<TransformComponent>().rotation = transform->rotation;
        }
    }

    /*
        Entity* gun = rel->getChildren("gun");

*/


}
