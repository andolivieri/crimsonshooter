#include "weaponcomponent.h"
#include "projectilecomponent.h"
#include "tilecomponent.h"

WeaponComponent::WeaponComponent(const std::string n):
    name(n)
{

}

void WeaponComponent::init()
{

    if(!entity->hasComponent<TransformComponent>())
        entity->addComponent<TransformComponent>();
    transform = &entity->getComponent<TransformComponent>();

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
            angle *= std::pow(-1, i);
            Vector2D randpoint = Math2D::rotate_point(transform->center(), angle, {mousePt.x, mousePt.y});
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
