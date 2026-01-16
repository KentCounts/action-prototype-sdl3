#include "Bullet.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <SDL3_image/SDL_image.h>


// SDL_Texture* Bullet::texture = nullptr;
// int Bullet::FrameWidth = 0;
// int Bullet::FrameHeight = 0;

Bullet::Bullet(float x, float y, float directionX, float directionY, float speed)
{
	
	position = { x, y };

	float length = SDL_sqrtf(directionX * directionX + directionY * directionY);
	if (length > 0.0f)
	{
		directionX /= length;
		directionY /= length;
	}

	velocity = {directionX* speed, directionY* speed};
	// Angle = SDL_atan2f(directionY, directionX);

}

/*void Bullet::LoadBullettexture(SDL_Renderer* renderer, const char* path)
{
	SDL_Surface* surf = IMG_Load(path);
	if (!surf) return;

	texture = SDL_CreateTextureFromSurface(renderer, surf);

	FrameWidth = surf->w / 4;
	FrameHeight = surf->h;

	SDL_DestroySurface;
} */

void Bullet::update(float DeltaTime)
{
	position.x += velocity.x * DeltaTime;
	position.y += velocity.y * DeltaTime;

	/* FrameTimer += DeltaTime;
	if (FrameTimer >= FrameTime)
	{
		FrameTimer -= FrameTime;
		Frame = (Frame + 1) % 4;
	}*/

}

void Bullet::render(SDL_Renderer* renderer)
{
	// if (!texture) return;

	/*SDL_FRect src = {
		(float)(Frame * FrameWidth),
		0.0f,
		(float)(FrameWidth),
		(float)(FrameHeight)
	};*/

	// float scale = 2.0f;

	/* SDL_FRect dst = {
		position.x - FrameWidth * scale * 0.5f,
		position.y - FrameHeight * scale * 0.5f,
		FrameWidth * scale,
		FrameHeight * scale
	};*/

	// float AngleDegrees = Angle * 180.0f / 3.141592f + 90.0f;

	/*SDL_RenderTextureRotated(
		renderer,
		texture,
		&src,
		&dst,
		AngleDegrees,
		nullptr,
		SDL_FLIP_NONE
	);*/

	// Dot bullet render
	const float size = 4.0f;
	SDL_FRect dot = {
		position.x - size * 0.5f,
		position.y - size * 0.5f,
		size,
		size
	};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &dot);

}

bool Bullet::isOffScreen(int windowWidth, int windowHeight) const
{
	return position.x < -50 || position.x > windowWidth + 50 || position.y < -50 || position.y > windowHeight + 50;
}