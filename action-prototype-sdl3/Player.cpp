#include "Player.h"
#include <SDL3/SDL_keyboard.h>

Player::Player(float Xaxis, float Yaxis, float width, float height, float speed)
{
    rect = { Xaxis, Yaxis, width, height };
    this->speed = speed;
}

void Player::update(float DeltaTime, int windowWidth, int windowHeight)
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

    // new movement code
    rect.x += DeltaX * speed * DeltaTime;
    rect.y += DeltaY * speed * DeltaTime;


    // clamp movement to bounds of window
    if (rect.x < 0) rect.x = 0;
    if (rect.y < 0) rect.y = 0;
    if (rect.x + rect.w > windowWidth) rect.x = windowWidth - rect.w;
    if (rect.y + rect.h > windowHeight) rect.y = windowHeight - rect.h;
}

void Player::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
    SDL_RenderFillRect(renderer, &rect);
}
