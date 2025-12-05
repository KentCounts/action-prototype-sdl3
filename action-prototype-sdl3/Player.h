#pragma once
#include <SDL3/SDL.h>

class Player {
public:
    Player(float Xaxis, float Yaxis, float width, float height, float speed);

    void update(float DeltaTime, int windowWidth, int windowHeight);
    void render(SDL_Renderer* renderer);

private:
    SDL_FRect rect;
    float speed;
};
