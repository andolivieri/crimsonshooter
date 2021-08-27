#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#include <map>
#include "SDL.h"
#include "ecs.h"
#include "transformcomponent.h"
#include "texturemanager.h"
#include "animation.h"


class SpriteComponent : public Component
{
public:
    SpriteComponent() = default;
    SpriteComponent(const char* path);
    SpriteComponent(const char* path, const SDL_Rect& src);
    SpriteComponent(SDL_Texture* t);
    SpriteComponent(SDL_Texture* t, const SDL_Rect& src, SDL_RendererFlip flip);
    ~SpriteComponent();

    void init() override;
    void update() override;
    void draw() override;
    void play(const std::string& anim);
    void stop();

    SDL_Rect srcRect, dstRect;
    SDL_RendererFlip flip = SDL_FLIP_NONE;


    SpriteComponent& setSrcRect(const SDL_Rect& b);


    SpriteComponent& addAnimation(const char* name, const Animation& a);

private:

    std::map<std::string, Animation> m_animation;
    TransformComponent* transform;
    SDL_Texture *m_texture;

    bool m_animated = false;
    std::string m_currentAnimation;



    void _update();

};

#endif // SPRITECOMPONENT_H
