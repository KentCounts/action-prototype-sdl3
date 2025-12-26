#include "BackgroundLayer.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>

bool BackgroundLayer::Load(SDL_Renderer* renderer, const char* path, int frameW, int frameH, int frameCount)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        std::cout << "IMG_Load BG Error: " << SDL_GetError() << std::endl;
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);

    if (!texture) {
        std::cout << "CreateTexture BG Error: " << SDL_GetError() << std::endl;
        return false;
    }

    frameWidth = frameW;
    frameHeight = frameH;
    frames = frameCount;
    frame = 0;
    timer = 0.0f;

    return true;
}

void BackgroundLayer::Update(float dt)
{
    timer += dt;
    while (timer >= frameTime) {
        timer -= frameTime;
        frame = (frame + 1) % frames;
    }
}

void BackgroundLayer::Render(SDL_Renderer* renderer, int screenW, int screenH) const
{
    if (!texture) return;

    SDL_FRect src = {
        (float)(frame * frameWidth),
        0.0f,
        (float)frameWidth,
        (float)frameHeight
    };

    SDL_FRect dst = { 0.0f, 0.0f, (float)screenW, (float)screenH };

    SDL_RenderTexture(renderer, texture, &src, &dst);
}
