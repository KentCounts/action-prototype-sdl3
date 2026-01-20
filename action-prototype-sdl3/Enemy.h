#pragma once
#include <SDL3/SDL.h>

class Enemy
{
public:
    Enemy(float x, float y, float VelocityX, float VelocityY, float size);

    void update(float DeltaTime);
    void render(SDL_Renderer* renderer) const;
    bool isOffScreen(int windowWidth, int windowHeight) const;
    SDL_FPoint GetPos() const { return position; }
    float GetRadius() const { return size * 0.5f; }

    static void LoadEnemyTexture(SDL_Renderer* renderer, const char* path);
    static void LoadExplosionTexture(SDL_Renderer* renderer, const char* path);


    bool TriggerExplosion();
    bool IsFinished() const;

private:
    SDL_FPoint position;
    SDL_FPoint velocity;

    float size;
    float angle = 0.0f;
    float spinSpeed;

    static SDL_Texture* texture;

    bool exploding = false;

    int explodeFrame = 0;
    float explodeTimer = 0.0f;
    static constexpr float explodeFrameTime = 0.06f;

    static SDL_Texture* explodeTexture;
    static constexpr int ExplodeFrameSize = 96;
    static constexpr int ExplodeFrameCount = 8;

};
