#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H
#include "projectilecomponent.h"
#include "tilecomponent.h"
#include "inputcomponent.h"
#include "assetmanager.h"
#include "weaponfactory.h"
#include "fsmcomponent.h"
#include "projectilecomponent.h"

class WeaponComponent;

class WeaponStateBase : public FSM_StateBase
{
public:

    WeaponStateBase(const WeaponData& w, Entity& e):
        entity(e),
        weaponData(w)
    {
        sprite = &entity.getComponent<SpriteComponent>();
        sound = &entity.getComponent<SoundComponent>();
        input = &entity.getComponent<InputComponent>();
        weapon = &entity.getComponent<WeaponComponent>();
    }

    Entity& entity;

    InputComponent* input;
    SpriteComponent* sprite;
    SoundComponent* sound;
    WeaponComponent* weapon;
    WeaponData weaponData;
protected:
    int currentMagazineShotCount = 0;

};

class WeaponStateShooting;
class WeaponStateIdle;
class WeaponStateReloading;

class WeaponStateIdle : public WeaponStateBase
{
public:
    WeaponStateIdle(const WeaponData& w, Entity& e) : WeaponStateBase(w,e){}

    void onEnter()
    {
        sprite->play("idle");
    }

    FSM_StateBase* handleInput() override;


};

class WeaponStateShooting : public WeaponStateBase
{
public:
    WeaponStateShooting(const WeaponData& w, Entity& e) : WeaponStateBase(w,e){}

    void onEnter()
    {
        lastShot = 0;
        sound->play(weaponData.soundShoot, 0);
    }

    FSM_StateBase* handleInput() override;

    void onExit()
    {
        sound->play(weaponData.soundEndfire, 0);
    }
private:
    Uint32 lastShot;
};

class WeaponStateReloading :  public WeaponStateBase
{
public:
    WeaponStateReloading(const WeaponData& w, Entity& e) : WeaponStateBase(w,e){}

    void onEnter()
    {
        sprite->play("reload", 1);
        sound->play(weaponData.soundReload);
        startTime = SDL_GetTicks();
    }

    FSM_StateBase* handleInput();
private:
    Uint32 startTime;
};

class WeaponComponent : public Component
{
public:

    WeaponComponent(const WeaponData& w):
        weapondata(w)
    {
    }

    void init() override
    {
        input = &entity->getComponent<InputComponent>();
        transform = &entity->getComponent<TransformComponent>();
        state = new WeaponStateIdle(weapondata, *entity);
    }

    void update() override
    {

        auto nextstate = state->handleInput();

        if(nextstate!=state){
            state->onExit();
            nextstate->onEnter();
            delete state;
        }

        state = nextstate;


    }

    void createProjectiles()
    {

        SDL_Point mousePt;
        SDL_GetMouseState(&mousePt.x,&mousePt.y);

        for(int i=0; i<weapondata.projectileGauges; i++)
        {
            auto& e = entity->m_manager.addEntity();
            int angle = 0;
            if(weapondata.projectileSpreadAngle > 0)
                angle = rand() % weapondata.projectileSpreadAngle;
            angle *= static_cast<int>(std::pow(-1, i)); // flip sign
            Vector2D randpoint = Math2D::rotate_point(
                        transform->center(),
                        static_cast<float>(angle),
                        {mousePt.x, mousePt.y}
                        );
            e.addComponent<ProjectileComponent>(
                        transform->center(),
                        randpoint)
                    .setSize(8,8)
                    .setRange(weapondata.range);
            e.addComponent<SpriteComponent>(weapondata.projectileSprite)
                    .setSrcRect({2,2,2,2});
        }

    }

    int currentMagazineShotCount = 0;

private:

    WeaponData weapondata;
    InputComponent* input;
    TransformComponent* transform;
    FSM_StateBase* state = nullptr;

};
#endif // WEAPONCOMPONENT_H
