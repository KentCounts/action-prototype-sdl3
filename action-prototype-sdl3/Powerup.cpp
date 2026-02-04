#include "PowerUp.h"

static bool RectsIntersect(const SDL_FRect& a, const SDL_FRect& b)
{
    return !(a.x + a.w < b.x ||
        b.x + b.w < a.x ||
        a.y + a.h < b.y ||
        b.y + b.h < a.y);
}

PowerUp::PowerUp(PowerUpCategory category, float x, float y, float size)
    : category(category)
{
    rect = { x, y, size, size };

    // Placeholder color by category (no textures yet)
    switch (category)
    {
    case PowerUpCategory::Engine: color = SDL_FColor{ 0.2f, 0.8f, 1.0f, 1.0f }; break;
    case PowerUpCategory::Weapon: color = SDL_FColor{ 1.0f, 0.3f, 0.3f, 1.0f }; break;
    case PowerUpCategory::Shield: color = SDL_FColor{ 0.4f, 1.0f, 0.4f, 1.0f }; break;
    }
}

void PowerUp::Update(float dt)
{
    // need to add animation, float and bob, etc. 
}

void PowerUp::Render(SDL_Renderer* renderer) const
{
    // Placeholder render
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer,
        (Uint8)(color.r * 255.0f),
        (Uint8)(color.g * 255.0f),
        (Uint8)(color.b * 255.0f),
        200);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &rect);
}

bool PowerUp::Intersects(const SDL_FRect& other) const
{
    return RectsIntersect(rect, other);
}

void PowerUp::ApplyTo(Loadout& loadout) const
{
    switch (category)
    {
    case PowerUpCategory::Engine:
        loadout.engineTier = ClampTier(loadout.engineTier + 1);
        break;
    case PowerUpCategory::Weapon:
        loadout.weaponTier = ClampTier(loadout.weaponTier + 1);
        break;
    case PowerUpCategory::Shield:
        loadout.shieldTier = ClampTier(loadout.shieldTier + 1);
        break;
    }
}
