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

    // movement conditionals
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
        rect.y -= speed * DeltaTime;

    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
        rect.y += speed * DeltaTime;

    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
        rect.x -= speed * DeltaTime;

    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
        rect.x += speed * DeltaTime;

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
