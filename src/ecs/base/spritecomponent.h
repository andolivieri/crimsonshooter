#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#include <map>
#include <functional>
#include "SDL.h"
#include "ecs/ecs.h"
#include "transformcomponent.h"
#include "engine/texturemanager.h"
#include "animation.h"
#include "helpers/stickerbaker.h"


class SpriteComponent : public Component
{
public:
    SpriteComponent() = default;
    SpriteComponent(const std::string& path);
    SpriteComponent(const std::string&, const SDL_Rect& src);
    SpriteComponent(SDL_Texture* t);
    SpriteComponent(SDL_Texture* t, const SDL_Rect& src, SDL_RendererFlip flip);
    ~SpriteComponent();

    void init() override;
    void update() override;
    void draw() override;
    void play(const std::string& anim, int repeat=-1);
    void stop();
    bool animationFinished() const;
    // TODO andoli: no this should be done stickerbaker somehow
    StickerOp snapshot(int group) const;

    bool absolute = false;
    SDL_Rect srcRect, dstRect;
    SDL_RendererFlip flip = SDL_FLIP_NONE;


    SpriteComponent& setSrcRect(const SDL_Rect& b);


    SpriteComponent& addAnimation(const char* name, const Animation& a);
    SpriteComponent& addAnimation(const char* name, const Animation& a, const uint32_t flags);
    SpriteComponent& setAlpha(uint8_t a);
    SpriteComponent& setColorMod(uint8_t r, uint8_t g, uint8_t b);
    SpriteComponent& setTransform(TransformComponent* t);
    SpriteComponent& setAbsolute(bool b);
    SpriteComponent& showFrame(bool b);
    SpriteComponent& setOnAnimationEnd(std::function<void()> cb);
private:

    std::map<std::string, Animation> m_animation;
    TransformComponent* transform;
    SDL_Texture *m_texture;

    bool m_animated = false;
    std::string m_currentAnimation;
    int m_animationLoops = 0; // 0  = infinite
    int m_animLoopCounter = 0;
    int m_animCurrentFrameIndex = 0;
    uint32_t lastTick = 0;
    int alpha = 255;
    uint8_t colorR = 255, colorG = 255, colorB = 255;
    bool _showFrame = false;

    std::function<void()> m_onAnimationEnd;
    bool m_animEndFired = false;



    void _update();

};

#endif // SPRITECOMPONENT_H
