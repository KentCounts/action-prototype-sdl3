#pragma once
#include <SDL3/SDL.h>

enum class PowerUpCategory
{
    Engine,
    Weapon,
    Shield
};

struct Loadout
{
    int engineTier = 1;
    int weaponTier = 1;
    int shieldTier = 1;
};

class PowerUp
{
public:

    // A powerup as an object
    PowerUp(PowerUpCategory category, float x, float y, float size = 48.0f);

    void Update(float dt);
    void Render(SDL_Renderer* renderer) const;

    // Collision
    SDL_FRect GetRect() const { return rect; }
    bool Intersects(const SDL_FRect& other) const;

    // Apply to loadout
    void ApplyTo(Loadout& loadout) const;

    PowerUpCategory GetCategory() const { return category; }

private:
    static int ClampTier(int t) { return (t < 1) ? 1 : (t > 4) ? 4 : t; }

    PowerUpCategory category;

    // placeholder box
    SDL_FRect rect;
    SDL_FColor color;
};
