#include "collidercomponent.h"
#include "engine/game.h"
#include "engine/texturemanager.h"
#include "engine/navgrid.h"
#include "helpers/collision.h"
#include <algorithm>
#include <cmath>

ColliderComponent::ColliderComponent(const std::string &t, int paddingX, int paddingY, float scale): tag(t)
{
    this->paddingX = paddingX;
    this->paddingY = paddingY;
    this->scale = scale;
}

ColliderComponent& ColliderComponent::setBlockedBySolids(std::vector<std::string> filters)
{
    blockedBySolids = true;
    blockFilters = std::move(filters);
    return *this;
}

ColliderComponent& ColliderComponent::setBlockedBySolids(const std::string& filter)
{
    return setBlockedBySolids(std::vector<std::string>{filter});
}

bool ColliderComponent::isBlockedBy(const std::string& solidTag) const
{
    if(blockFilters.empty()){
        return true;
    }

    bool hasWhitelist = false;
    bool whitelisted = false;
    for(const std::string& f : blockFilters){
        if(!f.empty() && f[0] == '!'){
            if(f.compare(1, std::string::npos, solidTag) == 0){
                return false; // blacklisted tag always wins
            }
        } else {
            hasWhitelist = true;
            if(f == solidTag){
                whitelisted = true;
            }
        }
    }

    return !hasWhitelist || whitelisted;
}

void ColliderComponent::init()
{
    if(!entity->hasComponent<TransformComponent>())
    {
        entity->addComponent<TransformComponent>();
    }
    entity->addGroup(groupColliders);
    transform = &entity->getComponent<TransformComponent>();

}

void ColliderComponent::update()
{
    
    if(blockedBySolids){
        resolveAgainstSolids();
    }

    collider.x = (int)transform->pos.x + paddingX;
    collider.y = (int)transform->pos.y + paddingY;
    collider.w = static_cast<int>(transform->width * scale);
    collider.h = static_cast<int>(transform->height  * scale);

    if(onCollisionCb == nullptr){
        return;
    }

    auto& otherColliders = entity->m_manager.getGroup(groupColliders);

    for(auto& c : otherColliders){

        if(c  != entity && c->hasComponent<ColliderComponent>()){
            ColliderComponent& targetCollider = c->getComponent<ColliderComponent>();
            if(Collision::AABB(*this, targetCollider))
            {
               onCollisionCb(*c);
            }
        }

    }

}

Vector2D ColliderComponent::circleCenter() const
{
    const float w = transform->width * scale;
    const float h = transform->height * scale;
    return Vector2D{ transform->pos.x + paddingX + w * 0.5f,
                     transform->pos.y + paddingY + h * 0.5f };
}

float ColliderComponent::circleRadius() const
{
    const float w = transform->width * scale;
    const float h = transform->height * scale;
    return (w < h ? w : h) * 0.5f;
}

bool ColliderComponent::pushIsInternal(int cc, int cr, const SDL_Rect& tile,
                                       const Vector2D& c) const
{
    const float left = static_cast<float>(tile.x);
    const float right = static_cast<float>(tile.x + tile.w);
    const float top = static_cast<float>(tile.y);
    const float bottom = static_cast<float>(tile.y + tile.h);

    // Which rect features the circle center is clamped against tells us the
    // contact: a face (one axis) or a corner (both). A face/corner is only a
    // real, exposed contact when the neighbouring cell in that direction is
    // open; if it's blocked, this is a shared tile seam and must be ignored.
    const int nx = c.x < left ? -1 : (c.x > right ? 1 : 0);
    const int ny = c.y < top ? -1 : (c.y > bottom ? 1 : 0);

    if (nx != 0 && ny != 0) {
        return NavGrid::isBlocked(cc + nx, cr) || NavGrid::isBlocked(cc, cr + ny);
    }
    if (nx != 0) {
        return NavGrid::isBlocked(cc + nx, cr);
    }
    if (ny != 0) {
        return NavGrid::isBlocked(cc, cr + ny);
    }
    return false; // center inside the tile: always resolve
}

void ColliderComponent::resolveCircleAgainstSolids()
{
    if (!NavGrid::ready()) {
        return;
    }

    const float radius = circleRadius();

    // A couple of passes let multi-tile contacts (e.g. inside corners) settle.
    for (int pass = 0; pass < 2; ++pass) {
        Vector2D c = circleCenter();

        int c0, r0, c1, r1;
        NavGrid::worldToCell(Vector2D{ c.x - radius, c.y - radius }, c0, r0);
        NavGrid::worldToCell(Vector2D{ c.x + radius, c.y + radius }, c1, r1);

        bool moved = false;
        for (int cr = r0; cr <= r1; ++cr) {
            for (int cc = c0; cc <= c1; ++cc) {
                for (ColliderComponent* solid : NavGrid::solidsAt(cc, cr)) {
                    if (solid == this || !solid->isSolid || !isBlockedBy(solid->tag)) {
                        continue;
                    }
                    Vector2D push;
                    if (!Collision::circleVsRect(c, radius, solid->collider, push)) {
                        continue;
                    }
                    if (pushIsInternal(cc, cr, solid->collider, c)) {
                        continue;
                    }
                    transform->pos.x += push.x;
                    transform->pos.y += push.y;
                    c.x += push.x;
                    c.y += push.y;
                    moved = true;
                }
            }
        }

        if (!moved) {
            break;
        }
    }
}

void ColliderComponent::resolveAgainstSolids()
{
    if (isCircle) {
        resolveCircleAgainstSolids();
        return;
    }

    // rolls back TransformComponent on per-axis base
    const float velX = transform->velocity.x;
    const float velY = transform->velocity.y;
    if(velX == 0 && velY == 0){
        return;
    }

    const float prevX = transform->pos.x - velX;
    const float prevY = transform->pos.y - velY;
    const int w = static_cast<int>(transform->width * scale);
    const int h = static_cast<int>(transform->height * scale);

    auto rectAt = [&](float px, float py) {
        return SDL_Rect{ static_cast<int>(px) + paddingX,
                         static_cast<int>(py) + paddingY, w, h };
    };

    auto& all = entity->m_manager.getGroup(groupColliders);
    auto hitsSolid = [&](const SDL_Rect& r) {
        for(auto& c : all){
            if(c == entity || !c->hasComponent<ColliderComponent>()){
                continue;
            }
            ColliderComponent& other = c->getComponent<ColliderComponent>();
            if(other.isSolid && isBlockedBy(other.tag) && Collision::AABB(r, other.collider)){
                return true;
            }
        }
        return false;
    };

    float finalX = transform->pos.x;
    if(velX != 0 && hitsSolid(rectAt(finalX, prevY))){
        finalX = prevX;
    }

    float finalY = transform->pos.y;
    if(velY != 0 && hitsSolid(rectAt(finalX, finalY))){
        finalY = prevY;
    }

    transform->pos.x = finalX;
    transform->pos.y = finalY;
}

void ColliderComponent::draw()
{
    if(!Game::debugColliders){
        return;
    }

    SDL_SetRenderDrawColor(TextureManager::renderer,
                           isSolid ? 0 : 255, isSolid ? 255 : 0, 0, 255);

    if(isCircle){
        const Vector2D c = circleCenter();
        const int cx = static_cast<int>(c.x) - Game::camera.x;
        const int cy = static_cast<int>(c.y) - Game::camera.y;
        const int rad = static_cast<int>(circleRadius());
        constexpr float TWO_PI = 6.2831853f;
        constexpr int segments = 24;
        for(int i = 0; i < segments; ++i){
            const float a0 = TWO_PI * i / segments;
            const float a1 = TWO_PI * (i + 1) / segments;
            SDL_RenderDrawLine(TextureManager::renderer,
                cx + static_cast<int>(std::cos(a0) * rad),
                cy + static_cast<int>(std::sin(a0) * rad),
                cx + static_cast<int>(std::cos(a1) * rad),
                cy + static_cast<int>(std::sin(a1) * rad));
        }
    }else{

        SDL_Rect r = collider;
        r.x -= Game::camera.x;
        r.y -= Game::camera.y;
        SDL_RenderDrawRect(TextureManager::renderer, &r);
    }

}

ColliderComponent& ColliderComponent::onCollision(std::function<void(Entity& target)> p)
{
    onCollisionCb = p;
    return *this;
}

