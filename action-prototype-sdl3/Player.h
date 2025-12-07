#pragma once
#include <SDL3/SDL.h>

class Player {
public:
    Player(float Xaxis, float Yaxis, float width, float height, float speed);

    void update(float DeltaTime, int windowWidth, int windowHeight);
    void render(SDL_Renderer* renderer);

    float getXCenter() const { return rect.x + rect.w / 2; }
    float getYCenter() const { return rect.y + rect.h / 2; }


private:
    SDL_FRect rect;
    float speed;
    float velocityX = 0.0f;
    float velocityY = 0.0f;
};
