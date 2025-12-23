#pragma once
#include <SDL3/SDL.h>

class Bullet {
public:
    Bullet(float x, float y, float directionX, float directionY, float speed);

    void update(float dt);
    void render(SDL_Renderer* renderer);

    SDL_FPoint GetPos() const { return position; }
    float GetRadius() const { return 6.0f; }

    bool isOffScreen(int winW, int winH) const;

    static void LoadBullettexture(SDL_Renderer* renderer, const char* path);

private:
    SDL_FRect rect;
    float dx;
    float dy;
    float speed;

    SDL_FPoint position;
    SDL_FPoint velocity;

    float Angle = 0.0f;

    int Frame = 0;
    float FrameTimer = 0.0f;
    float FrameTime = 0.05f;

    static SDL_Texture* texture;
    static int FrameWidth;
    static int FrameHeight;
};
