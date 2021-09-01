#include "weaponcomponent.h"
#include "projectilecomponent.h"
#include "tilecomponent.h"

WeaponComponent::WeaponComponent(const std::string& n):
    currentweapon(n)
{

}

void WeaponComponent::init()
{

    if(!entity->hasComponent<TransformComponent>())
        entity->addComponent<TransformComponent>();
    transform = &entity->getComponent<TransformComponent>();


    if(!entity->hasComponent<RelationshipComponent>())
        entity->addComponent<RelationshipComponent>();
    rel = &entity->getComponent<RelationshipComponent>();

    equip(currentweapon);

}

void WeaponComponent::equip(const std::string& n)
{
    // TODO weapon factory
    auto& gun = entity->m_manager.addEntity();
    gun.addComponent<SpriteComponent>("assets/shotgun.png")
            .addAnimation("idle", {0, 1, 100 })
            .addAnimation("shooting", {0, 11, 100});
    rel->addChildren(&gun, "gun");

}

void WeaponComponent::update()
{
    if(shooting && SDL_GetTicks() - lastShot > cooldown)
    {

        SDL_Point mousePt;
        SDL_GetMouseState(&mousePt.x,&mousePt.y);

        for(int i=0; i<12; i++)
        {
            auto& e = entity->m_manager.addEntity();
            int angle = rand() % 30;
            angle *= static_cast<int>(std::pow(-1, i)); // flip sign
            Vector2D randpoint = Math2D::rotate_point(
                        transform->center(),
                        static_cast<float>(angle),
            {mousePt.x, mousePt.y});
            e.addComponent<ProjectileComponent>(
                        transform->center(),
                        randpoint)
                    .setSize(8,8).setRange(500);
            e.addComponent<SpriteComponent>("assets/projectile.png")
                    .setSrcRect({2,2,2,2});
        }


        lastShot = SDL_GetTicks();

    }


}

void WeaponComponent::triggerPull()
{
    shooting = true;
}

void WeaponComponent::triggerRelease()
{
    shooting = false;
}
