#include "weaponbaycomponent.h"
#include "projectilecomponent.h"
#include "tilecomponent.h"
#include "assetmanager.h"
#include "weaponfactory.h"
#include "fsmcomponent.h"

WeaponBayComponent::WeaponBayComponent(const std::string& n):
    currentweapon(n)
{

}

WeaponBayComponent &WeaponBayComponent::setAttachPoint(const Vector2D &a)
{
    attachPoint = a;
    return *this;
}

void WeaponBayComponent::init()
{

    transform = &entity->getComponent<TransformComponent>();
    input = &entity->getComponent<InputComponent>();


    if(!entity->hasComponent<RelationshipComponent>())
        entity->addComponent<RelationshipComponent>();
    rel = &entity->getComponent<RelationshipComponent>();

    equip(currentweapon);

}

void WeaponBayComponent::equip(const std::string& weaponId)
{
    auto& gun = WeaponFactory(entity->m_manager).createWeaponEntity(weaponId);
    if(rel->hasChildren("gun")){
        auto g = rel->getChildren("gun");
        rel->removeChild(g);
        g->setActive(false);
    }

    rel->addChildren(&gun, "gun");

}

void WeaponBayComponent::update()
{

    Entity* gun = rel->getChildren("gun");

    auto center = transform->center();
    center.x += attachPoint.x;
    center.y += attachPoint.y;
    Vector2D rotatedCenter = Math2D::rotate_point(transform->center(), transform->rotation, center);
    gun->getComponent<TransformComponent>().centerOn(rotatedCenter);
    gun->getComponent<TransformComponent>().rotation = transform->rotation;

}

void WeaponBayComponent::triggerPull()
{
    shooting = true;
}

void WeaponBayComponent::triggerRelease()
{
    shooting = false;
}

