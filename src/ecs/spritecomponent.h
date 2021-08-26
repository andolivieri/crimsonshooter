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
    ~SpriteComponent();

    void init() override;
    void update() override;
    void draw() override;

private:
    TransformComponent* transform;
    SDL_Texture *m_texture;
    SDL_Rect srcRect, dstRect;


    void _update();

};

#endif // SPRITECOMPONENT_H
