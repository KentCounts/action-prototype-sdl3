#include "Player.h"
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>


Player::Player(float Xaxis, float Yaxis, float width, float height, float speed)
{
    rect = { Xaxis, Yaxis, width, height };
    this->speed = speed;
}

void Player::update(float DeltaTime, int windowWidth, int windowHeight, float MouseX, float MouseY)
{


    
    const bool* keys = SDL_GetKeyboardState(nullptr);

    float DeltaX = 0.0f;
    float DeltaY = 0.0f;

    // movement conditionals
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
        DeltaY -= 1.0f;

    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
        DeltaY += 1.0f;

    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
        DeltaX -= 1.0f;

    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
        DeltaX += 1.0f;

    float length = SDL_sqrtf(DeltaX * DeltaX + DeltaY * DeltaY);

    if (length > 0.0f)
    {
        DeltaX /= length;
        DeltaY /= length;
    }

    // calculate angle toward mouse
    float cx = rect.x + rect.w * 0.5f;
    float cy = rect.y + rect.h * 0.5f;

    float dx = MouseX - cx;
    float dy = MouseY - cy;

    angle = SDL_atan2f(dy, dx);


    // new movement code
    rect.x += DeltaX * speed * DeltaTime;
    rect.y += DeltaY * speed * DeltaTime;


    // clamp movement to bounds of window
    if (rect.x < 0) rect.x = 0;
    if (rect.y < 0) rect.y = 0;
    if (rect.x + rect.w > windowWidth) rect.x = windowWidth - rect.w;
    if (rect.y + rect.h > windowHeight) rect.y = windowHeight - rect.h;
}

void Player::LoadShiptexture(SDL_Renderer* renderer, const char* path)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
        return;
    }

    Shiptexture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);

    if (!Shiptexture) {
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }
}

void Player::LoadEnginetexture(SDL_Renderer* renderer, const char* path)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        std::cout << "IMG_Load Engine Error: " << SDL_GetError() << std::endl;
        return;
    }

    Enginetexture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);

    if (!Enginetexture) {
        std::cout << "SDL_CreateTextureFromSurface Engine Error: " << SDL_GetError() << std::endl;
    }
}

void Player::render(SDL_Renderer* renderer)
{

    float AngleDegrees = angle * 180.0f / 3.141592 + 90.0f;

    
    
    if (!Shiptexture)
    {
        return;
    }

    SDL_FRect dest = rect;

    SDL_RenderTextureRotated(
        renderer,
        Shiptexture,
        nullptr,
        &dest,
        AngleDegrees,
        nullptr,
        SDL_FLIP_NONE
    );

    if (!Enginetexture)
    {
        return;
    }

    SDL_FRect engine = rect;

    float rotX = engineOffsetX * SDL_cosf(angle) - engineOffsetY * SDL_sinf(angle);
    float rotY = engineOffsetX * SDL_sinf(angle) + engineOffsetY * SDL_cosf(angle);


    engine.x += rotX;
    engine.y += rotY;

    SDL_RenderTextureRotated(
        renderer,
        Enginetexture,
        nullptr,
        &engine,
        AngleDegrees,
        nullptr,
        SDL_FLIP_NONE
    );
   
}
