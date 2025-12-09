#include "Bullet.h"
#include <SDL3/SDL.h>
#include <cmath>

Bullet::Bullet(float x, float y, float directionX, float directionY, float speed)
{
	// small dot object for bullet
	rect = { x, y, 6.0f, 6.0f };
	this->speed = speed;

	float length = std::sqrt(directionX * directionX + directionY * directionY);
	dx = directionX / length;
	dy = directionY / length;

}

void Bullet::update(float DeltaTime)
{
	rect.x += dx * speed * DeltaTime;
	rect.y += dy * speed * DeltaTime;

}

void Bullet::render(SDL_Renderer* renderer)
{
	// yellow
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);

}

bool Bullet::isOffScreen(int windowWidth, int windowHeight) const
{
	return (rect.x < -10 || rect.y < -10 || rect.x > windowWidth + 10 || rect.y > windowHeight + 10);
}