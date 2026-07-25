#include "weaponcomponent.h"
#include <ecs/effects/soundcomponent.h>
#include <engine/game.h>
#include <cmath>


class WeaponStateBase : public FSM_StateBase
{
public:

    WeaponStateBase(const WeaponData& w, Entity& e):
        entity(e),
        weaponData(w)
    {
        sprite = &entity.getComponent<SpriteComponent>();
        sound = &entity.getComponent<SoundComponent>();
        weapon = &entity.getComponent<WeaponComponent>();
    }

    Entity& entity;

    SpriteComponent* sprite;
    SoundComponent* sound;
    WeaponComponent* weapon;
    const WeaponData& weaponData;
protected:
    int currentMagazineShotCount = 0;

};

class WeaponStateShooting;
class WeaponStateIdle;
class WeaponStateReloading;



/////////////////////////////////////////////////////////////////
///                 IDLE                                       //
/////////////////////////////////////////////////////////////////

class WeaponStateIdle : public WeaponStateBase
{
public:
    WeaponStateIdle(const WeaponData& w, Entity& e) : WeaponStateBase(w,e){}

    void onEnter()
    {
        sprite->play("idle");
        sound->play(weaponData.soundIdle, -1, 1);
    }

    FSM_StateBase* handleInput() override;


};

/////////////////////////////////////////////////////////////////
///                 SHOOTING                                   //
/////////////////////////////////////////////////////////////////
class WeaponStateShooting : public WeaponStateBase
{
public:
    WeaponStateShooting(const WeaponData& w, Entity& e) : WeaponStateBase(w,e){}

    void onEnter()
    {
        lastShot = 0;
        sprite->play("shoot", weaponData.automatic ? -1 : 1);
        if(weaponData.chainsaw)
            sound->play(weaponData.soundShoot, -1 , 1);
    }

    FSM_StateBase* handleInput() override;

    void onExit()
    {
        sound->play(weaponData.soundEndfire, 0, weaponData.chainsaw ? 2 : 1);
    }

    void shoot();
private:
    Uint32 lastShot;
};

/////////////////////////////////////////////////////////////////
///                 REALOADING                                 //
/////////////////////////////////////////////////////////////////

class WeaponStateReloading :  public WeaponStateBase
{
public:
    WeaponStateReloading(const WeaponData& w, Entity& e) : WeaponStateBase(w,e){}

    void onEnter()
    {
        sprite->play("reload", 1);
        sound->play(weaponData.soundReload, 0, 2);
        startTime = SDL_GetTicks();
    }

    FSM_StateBase* handleInput();
private:
    Uint32 startTime;
};





void WeaponComponent::init()
{
    transform = &entity->getComponent<TransformComponent>();
    state = new WeaponStateIdle(weapondata, *entity);
    state->onEnter();
}

void WeaponComponent::triggerPull()
{
    triggerDown = true;
}

void WeaponComponent::triggerRelease()
{
    triggerDown = false;
}

void WeaponComponent::reload()
{
    reloadRequested = true;
}

void WeaponComponent::update()
{

    auto nextstate = state->handleInput();

    if(nextstate!=state){
        state->onExit();
        nextstate->onEnter();
        delete state;
    }

    state = nextstate;


}


void WeaponStateShooting::shoot()
{
    if(!weaponData.chainsaw)
        sound->play(weaponData.soundShoot, weaponData.automatic ? -1 : 0, 1);
    weapon->createProjectiles();
    weapon->currentMagazineShotCount++;
}


void WeaponComponent::createProjectiles()
{

    Vector2D bulletStart = transform->center();
    bulletStart.x += weapondata.muzzlePos.x;
    bulletStart.y += weapondata.muzzlePos.y;
    bulletStart = Math2D::rotate_point(
                transform->center(),
                transform->rotation,
                bulletStart);

    // both works for mouse and joystick
    double aimRad = Math2D::deg2rad(transform->rotation);
    Vector2D aimTarget{
        bulletStart.x + static_cast<float>(std::cos(aimRad)) * 1000.0f,
        bulletStart.y + static_cast<float>(std::sin(aimRad)) * 1000.0f
    };

    for(int i=0; i<weapondata.projectileGauges; i++)
    {
        auto& e = entity->m_manager.addEntity();
        int angle = 0;
        if(weapondata.projectileSpreadAngle > 0)
            angle = rand() % weapondata.projectileSpreadAngle;
        angle *= static_cast<int>(std::pow(-1, i)); // flip sign


        Vector2D randpoint = Math2D::rotate_point(
                    bulletStart,
                    static_cast<float>(angle),
                    aimTarget
                    );
        e.addComponent<ProjectileComponent>(
                    bulletStart,
                    randpoint)
                .setDamage(weapondata.projectileDamage)
                .setSize(8,8)
                .setRange(weapondata.range);
        //e.addComponent<ColliderComponent>().onCollision([&])

        if(weapondata.projectileSprite.length())
            e.addComponent<SpriteComponent>(weapondata.projectileSprite)
                    .setSrcRect({2,2,2,2});

        if(weapondata.tracerEnabled && (tracerCounter++ % weapondata.tracerEvery == 0)){
            TrailComponent::Config c;
            c.color = weapondata.tracerColor;
            c.maxPoints = weapondata.tracerLength;
            c.thickness = weapondata.tracerTickness;
            e.addComponent<TrailComponent>(c);
        }
    }

}

Vector2D WeaponComponent::getAttachMargin()
{
    return weapondata.attachMargin;
}

WeaponData& WeaponComponent::wpData() {
    return weapondata;
}



/////////////////////////////////////////////////////////////////
///                 SHOOTING                                   //
/////////////////////////////////////////////////////////////////
FSM_StateBase *WeaponStateShooting::handleInput()
{

    if(weapon->currentMagazineShotCount >= weaponData.magazine)
    {
        return new WeaponStateReloading(weaponData, entity);
    }

    // Trigger released => back to idle.
    if(!weapon->triggerDown)
        return new WeaponStateIdle(weaponData, entity);

    if(weaponData.chainsaw)
        return this;

    if(SDL_GetTicks() - lastShot >= (60.0 / weaponData.rate) * ONE_SECOND)
    {
        shoot();
        lastShot = SDL_GetTicks();
    }
    return this;
}

/////////////////////////////////////////////////////////////////
///                 RELOADING                                  //
/////////////////////////////////////////////////////////////////
FSM_StateBase *WeaponStateReloading::handleInput()
{
    // no event accepted just wait for reload to complete
    if(SDL_GetTicks() - startTime > weaponData.reloadTimeMsec)
    {
        weapon->currentMagazineShotCount = 0;
        return new WeaponStateIdle(weaponData, entity);
    }
    return this;
}
/////////////////////////////////////////////////////////////////
///                 IDLE                                       //
/////////////////////////////////////////////////////////////////
FSM_StateBase *WeaponStateIdle::handleInput()
{
    // Manual reload request => reloading.
    if(weapon->reloadRequested)
    {
        weapon->reloadRequested = false;
        return new WeaponStateReloading(weaponData, entity);
    }
    // Trigger pulled => shooting.
    if(weapon->triggerDown)
        return new WeaponStateShooting(weaponData, entity);
    return this;
}
