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

private:
    SDL_FPoint position;
    SDL_FPoint velocity;

    float size;
    float angle = 0.0f;
    float spinSpeed;

    static SDL_Texture* texture;
};
