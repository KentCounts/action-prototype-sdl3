#pragma once
#include <SDL3/SDL.h>

class BackgroundLayer
{
public:
    bool Load(SDL_Renderer* renderer, const char* path, int frameW, int frameH, int frameCount);
    void Update(float dt);
    void Render(SDL_Renderer* renderer, int screenW, int screenH) const;

    void SetFrameTime(float secondsPerFrame) { frameTime = secondsPerFrame; }

private:
    SDL_Texture* texture = nullptr;

    int frameWidth = 0;
    int frameHeight = 0;
    int frames = 0;

    int frame = 0;
    float timer = 0.0f;
    float frameTime = 0.08f;
};
