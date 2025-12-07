#pragma once
#include <SDL3/SDL.h>

class Bullet {
public:
    Bullet(float x, float y, float directionX, float directionY, float speed);

    void update(float dt);
    void render(SDL_Renderer* renderer);

    bool isOffScreen(int winW, int winH) const;

private:
    SDL_FRect rect;
    float dx;
    float dy;
    float speed;
};
