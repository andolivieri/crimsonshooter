#include "weaponcomponent.h"
#include "projectilecomponent.h"
#include "tilecomponent.h"
#include "assetmanager.h"

WeaponComponent::WeaponComponent(const std::string& n):
    currentweapon(n)
{

}

WeaponComponent &WeaponComponent::setAttachPoint(const Vector2D &a)
{
    attachPoint = a;
    return *this;
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
    SDL_UNUSED(n);
    // TODO weapon factory
    auto& gunsprite = entity->m_manager.addEntity();

    gunsprite.addComponent<TransformComponent>();
    gunsprite.getComponent<TransformComponent>().width = 64;
    gunsprite.getComponent<TransformComponent>().height = 64;
    gunsprite.addComponent<SpriteComponent>("assets/shotgun.png")
            .addAnimation("idle", {0, 1, 100 })
            .addAnimation("shooting", {0, 12, 100});
    gunsprite.addGroup(groupWeapons);
    rel->addChildren(&gunsprite, "gun");

}

void WeaponComponent::update()
{

    Entity* gun = rel->getChildren("gun");

    auto center = transform->center();
    center.x += attachPoint.x;
    center.y += attachPoint.y;
    Vector2D rotatedCenter = Math2D::rotate_point(transform->center(), transform->rotation, center);
    gun->getComponent<TransformComponent>().centerOn(rotatedCenter);
    gun->getComponent<TransformComponent>().rotation = transform->rotation;


    std::string anim = "idle";


    bool inCooldown = SDL_GetTicks() - lastShot < cooldown;

    if(inCooldown)
        anim = "shooting";

    if(shooting && !inCooldown)
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

        auto sound = AssetManager::getSound("assets/shotgun.wav");
        Mix_PlayChannel(-1, sound, 0);


        lastShot = SDL_GetTicks();

    }

    if(shooting)
        anim = "shooting";

    gun->getComponent<SpriteComponent>().play(anim);


}

void WeaponComponent::triggerPull()
{
    shooting = true;
}

void WeaponComponent::triggerRelease()
{
    shooting = false;
}
