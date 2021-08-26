#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#include "SDL.h"
#include "ecs.h"
#include "transformcomponent.h"
#include "texturemanager.h"


class SpriteComponent : public Component
{
public:
    SpriteComponent() = default;
    SpriteComponent(const char* path);
    SpriteComponent(SDL_Texture* t);
    SpriteComponent(SDL_Texture* t, const SDL_Rect& src, SDL_RendererFlip flip);
    SpriteComponent(const char* path, const SDL_Rect& src);
    ~SpriteComponent();

    void init() override;
    void update() override;
    void draw() override;

    SDL_Rect srcRect, dstRect;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

private:
    TransformComponent* transform;
    SDL_Texture *m_texture;


    void _update();

};

#endif // SPRITECOMPONENT_H
