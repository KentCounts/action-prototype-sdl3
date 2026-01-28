#pragma once
#include <SDL3/SDL.h>

class Player {
public:
    Player(float Xaxis, float Yaxis, float width, float height, float speed);

    void update(float DeltaTime, int windowWidth, int windowHeight, float MouseX, float MouseY);
    void render(SDL_Renderer* renderer);

    float getXCenter() const { return rect.x + rect.w / 2; }
    float getYCenter() const { return rect.y + rect.h / 2; }

    void LoadShipTextures(SDL_Renderer* renderer,
        const char* ship1,
        const char* ship2,
        const char* ship3,
        const char* ship4);

    // void LoadEnginetexture(SDL_Renderer* renderer, const char* path);
    // void LoadGuntexture(SDL_Renderer* renderer, const char* path);
    // void LoadFlametexture(SDL_Renderer* renderer, const char* path);

    // void SetFiring(bool firing) { IsFiring = firing; }

    // SDL_FPoint GetLeftGunPos() const;
    // SDL_FPoint GetRightGunPos() const;

    // bool FireLeftNext = true;

    void ResetHealth(int maxHp = 4);
    bool TakeHit(int dmg = 1);              
    void Tick(float dt);                    
    int  GetHealth() const { return health; }
    int  GetMaxHealth() const { return maxHealth; }
    bool IsDead() const { return health <= 0; }

    // Collision helper (keeps rect private)
    float GetCollisionRadius(float tightness = 0.85f) const;


private:
    SDL_FRect rect;
    float speed;
    float velocityX = 0.0f;
    float velocityY = 0.0f;

    SDL_Texture* Shiptexture = nullptr;

    void UpdateShipTexture();

    SDL_Texture* Ship1 = nullptr;
    SDL_Texture* Ship2 = nullptr;
    SDL_Texture* Ship3 = nullptr;
    SDL_Texture* Ship4 = nullptr;

    // SDL_Texture* Enginetexture = nullptr;
    // SDL_Texture* Guntexture = nullptr;
    // SDL_Texture* Flametexture = nullptr;

    float angle = 0.0f;
    // float engineOffsetX = -20.0f;
    // float engineOffsetY = 0.0f;

    // int GunFrame = 0;
    // const int GunFrameCount = 7;
    // float GunFrameTime = 0.05f;
    // float GunTimer = 0.0f;
    // bool IsFiring = false;
    // int GunFrameWidth = 0;
    // int GunFrameHeight = 0;
    // SDL_FPoint LeftGunOffset = { -8.0f, -20.0f };
    // SDL_FPoint RightGunOffset = { -8.0f, 20.0f };


    // int FlameFrame = 0;
    // float FlameTimer = 0.0f;
    // float FlameFrameTime = 0.05f;
    // int FlameFrameWidth = 0;
    // int FlameFrameHeight = 0;
    // const int FlameFrameCount = 4;

    // float flameOffsetX = -10.0f;
    // float flameOffsetY = 0.0f;
    // float flameScale = 1.0f;

    // Health 
    int health = 4;
    int maxHealth = 4;

    float hitCooldown = 0.0f;
    float hitCooldownDuration = 0.6f;

    int lastHealthForSprite = 4;
};
