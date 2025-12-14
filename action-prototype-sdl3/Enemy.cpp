#include "Enemy.h"
#include <cmath>
#include <SDL3_image/SDL_image.h>
#include <cstdlib>

SDL_Texture* Enemy::texture = nullptr;

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

void Enemy::update(float dt)
{
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
    if (!texture) return;

    SDL_FRect dst = {
        position.x - size * 0.5f,
        position.y - size * 0.5f,
        size,
        size
    };

    float AngleDegrees = angle * 180.0f / 3.14159265f;

    SDL_RenderTextureRotated(
        renderer,
        texture,
        nullptr,
        &dst,
        AngleDegrees,
        nullptr,
        SDL_FLIP_NONE
    );
}
