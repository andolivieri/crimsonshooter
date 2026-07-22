#include "collidercomponent.h"
#include "engine/game.h"
#include "engine/texturemanager.h"
#include "helpers/collision.h"

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

void ColliderComponent::resolveAgainstSolids()
{
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

    SDL_Rect r = collider;
    r.x -= Game::camera.x;
    r.y -= Game::camera.y;
    SDL_SetRenderDrawColor(TextureManager::renderer,
                           isSolid ? 0 : 255, isSolid ? 255 : 0, 0, 255);
    SDL_RenderDrawRect(TextureManager::renderer, &r);
}

ColliderComponent& ColliderComponent::onCollision(std::function<void(Entity& target)> p)
{
    onCollisionCb = p;
    return *this;
}

