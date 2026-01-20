#include "Enemy.h"
#include <cmath>
#include <SDL3_image/SDL_image.h>
#include <cstdlib>

SDL_Texture* Enemy::texture = nullptr;
SDL_Texture* Enemy::explodeTexture = nullptr;


Enemy::Enemy(float x, float y, float VelocityX, float VelocityY, float size)
{
    position = { x, y };
    velocity = { VelocityX, VelocityY };
    this->size = size;

    spinSpeed = ((rand() % 200) - 100) * 0.01f;
}

void Enemy::LoadEnemyTexture(SDL_Renderer* renderer, const char* path)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) return;

    texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);
}

void Enemy::LoadExplosionTexture(SDL_Renderer* renderer, const char* path)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) return;

    explodeTexture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);
}

void Enemy::update(float dt)
{
    if (exploding)
    {
        explodeTimer += dt;
        if (explodeTimer >= explodeFrameTime)
        {
            explodeTimer -= explodeFrameTime;
            explodeFrame++;
        }
        return;
    }

    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    angle += spinSpeed * dt;
}

bool Enemy::isOffScreen(int w, int h) const
{
    return position.x < -size || position.x > w + size ||
        position.y < -size || position.y > h + size;
}

void Enemy::render(SDL_Renderer* renderer) const
{
    if (exploding)
    {
        if (!explodeTexture) return;

        SDL_FRect src = {
            explodeFrame * ExplodeFrameSize,
            0,
            ExplodeFrameSize,
            ExplodeFrameSize
        };

        SDL_FRect dst = {
            position.x - size * 0.5f,
            position.y - size * 0.5f,
            size,
            size
        };

        SDL_RenderTexture(renderer, explodeTexture, &src, &dst);
        return;
    }

    if (!texture) return;

    SDL_FRect dst = {
        position.x - size * 0.5f,
        position.y - size * 0.5f,
        size,
        size
    };

    float angleDeg = angle * 180.0f / 3.14159265f;

    SDL_RenderTextureRotated(
        renderer,
        texture,
        nullptr,
        &dst,
        angleDeg,
        nullptr,
        SDL_FLIP_NONE
    );
}


bool Enemy::TriggerExplosion()
{
    if (exploding) return false;
    exploding = true;
    explodeFrame = 0;
    explodeTimer = 0.0f;
}

bool Enemy::IsFinished() const
{
    return exploding && explodeFrame >= ExplodeFrameCount;
}

