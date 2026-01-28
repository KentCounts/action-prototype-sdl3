#include "Player.h"
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

void Player::ResetHealth(int maxHp)
{
    if (maxHp < 1) maxHp = 1;
    maxHealth = maxHp;
    health = maxHp;

    lastHealthForSprite = health;
    UpdateShipTexture();


    hitCooldown = 0.0f;
}

bool Player::TakeHit(int dmg)
{
    if (dmg <= 0) return false;
    if (health <= 0) return false;

    // i frames
    if (hitCooldown > 0.0f) return false;

    health -= dmg;
    if (health < 0) health = 0;

    if (health != lastHealthForSprite)
    {
        lastHealthForSprite = health;
        UpdateShipTexture();

    }

    hitCooldown = hitCooldownDuration;

    return true;
}

void Player::UpdateShipTexture()
{
    switch (health)
    {
    case 4:
        Shiptexture = Ship1;
        break;
    case 3:
        Shiptexture = Ship2;
        break;
    case 2:
        Shiptexture = Ship3;
        break;
    case 1:
        Shiptexture = Ship4;
        break;
    default:
        Shiptexture = Ship4; // dead / fallback
        break;
    }
}

static SDL_Texture* PickShipTexture(int health,
    SDL_Texture* ship1,
    SDL_Texture* ship2,
    SDL_Texture* ship3,
    SDL_Texture* ship4)
{
    switch (health)
    {
    case 4: return ship1;
    case 3: return ship2;
    case 2: return ship3;
    case 1: return ship4;
    default: return ship4;
    }
}

static SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* path)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf)
    {
        std::cout << "IMG_Load Error: " << SDL_GetError() << " path=" << path << std::endl;
        return nullptr;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);

    if (!tex)
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << " path=" << path << std::endl;

    return tex;
}


Player::Player(float Xaxis, float Yaxis, float width, float height, float speed)
{
    rect = { Xaxis, Yaxis, width, height };
    this->speed = speed;
}

void Player::update(float DeltaTime, int windowWidth, int windowHeight, float MouseX, float MouseY)
{

    if (hitCooldown > 0.0f)
    {
        hitCooldown -= DeltaTime;
        if (hitCooldown < 0.0f) hitCooldown = 0.0f;
    }
    
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

    /*if (IsFiring)
    {
        GunTimer += DeltaTime;

        if (GunTimer >= GunFrameTime)
        {
            GunTimer -= GunFrameTime;
            GunFrame++;

            if (GunFrame >= GunFrameCount)   // wrap around
                GunFrame = 0;
        }
    }
    else
    {
        GunFrame = 0;  // reset animation when not firing
        GunFrame = 0;
    } */

    /*FlameTimer += DeltaTime;
    if (FlameTimer >= FlameFrameTime)
    {
        FlameTimer -= FlameFrameTime;
        FlameFrame = (FlameFrame + 1) % FlameFrameCount;
    }*/

}

void Player::LoadShipTextures(SDL_Renderer* renderer,
    const char* ship1,
    const char* ship2,
    const char* ship3,
    const char* ship4)
{
    Ship1 = LoadTexture(renderer, ship1);
    Ship2 = LoadTexture(renderer, ship2);
    Ship3 = LoadTexture(renderer, ship3);
    Ship4 = LoadTexture(renderer, ship4);

    // Default to full-health appearance
    Shiptexture = Ship1;
}

/* void Player::LoadEnginetexture(SDL_Renderer* renderer, const char* path)
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
} */

/* void Player::LoadGuntexture(SDL_Renderer* renderer, const char* path)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        std::cout << "IMG_Load Gun Error: " << SDL_GetError() << std::endl;
        return;
    }

    Guntexture = SDL_CreateTextureFromSurface(renderer, surf);

    GunFrameWidth = surf->w / GunFrameCount;  
    GunFrameHeight = surf->h;


    SDL_DestroySurface(surf);

    if (!Guntexture) {
        std::cout << "SDL_CreateTextureFromSurface Gun Error: " << SDL_GetError() << std::endl;
    }
} */

/* void Player::LoadFlametexture(SDL_Renderer* renderer, const char* path)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        std::cout << "IMG_Load Flame Error: " << SDL_GetError() << std::endl;
        return;
    }

    Flametexture = SDL_CreateTextureFromSurface(renderer, surf);

    FlameFrameWidth = surf->w / FlameFrameCount;
    FlameFrameHeight = surf->h;

    SDL_DestroySurface(surf);

    if (!Flametexture) {
        std::cout << "SDL_CreateTextureFromSurface Flame Error: " << SDL_GetError() << std::endl;
    }
} */


/*SDL_FPoint Player::GetLeftGunPos() const
{
    float cx = rect.x + rect.w * 0.5f;
    float cy = rect.y + rect.h * 0.5f;

    // rotate offset
    float rx = LeftGunOffset.x * SDL_cosf(angle) - LeftGunOffset.y * SDL_sinf(angle);
    float ry = LeftGunOffset.x * SDL_sinf(angle) + LeftGunOffset.y * SDL_cosf(angle);

    return { cx + rx, cy + ry };
} */

/*SDL_FPoint Player::GetRightGunPos() const
{
    float cx = rect.x + rect.w * 0.5f;
    float cy = rect.y + rect.h * 0.5f;

    float rx = RightGunOffset.x * SDL_cosf(angle) - RightGunOffset.y * SDL_sinf(angle);
    float ry = RightGunOffset.x * SDL_sinf(angle) + RightGunOffset.y * SDL_cosf(angle);

    return { cx + rx, cy + ry };
} */


void Player::render(SDL_Renderer* renderer)
{

    float AngleDegrees = angle * 180.0f / 3.141592 + 90.0f;

    
    
    if (!Shiptexture)
    {
        return;
    }

    SDL_FRect ship = rect;

    SDL_RenderTextureRotated(
        renderer,
        Shiptexture,
        nullptr,
        &ship,
        AngleDegrees,
        nullptr,
        SDL_FLIP_NONE
    );

    /* if (!Enginetexture)
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
    ); */

    /*if (Flametexture)
    {
        SDL_FRect flameDst = rect;

        float rotX = engineOffsetX * SDL_cosf(angle) - engineOffsetY * SDL_sinf(angle);
        float rotY = engineOffsetX * SDL_sinf(angle) + engineOffsetY * SDL_cosf(angle);

        float frotX = flameOffsetX * SDL_cosf(angle) - flameOffsetY * SDL_sinf(angle);
        float frotY = flameOffsetX * SDL_sinf(angle) + flameOffsetY * SDL_cosf(angle);

        flameDst.x += rotX + frotX;
        flameDst.y += rotY + frotY;

        SDL_FRect flameSrc = {
            (float)(FlameFrame * FlameFrameWidth),
            0.0f,
            (float)FlameFrameWidth,
            (float)FlameFrameHeight
        };

        flameDst.w = rect.w * flameScale;
        flameDst.h = rect.h * flameScale;

        float AngleDegrees = angle * 180.0f / 3.14159265f + 90.0f;

        SDL_RenderTextureRotated(
            renderer,
            Flametexture,
            &flameSrc,
            &flameDst,
            AngleDegrees,
            nullptr,
            SDL_FLIP_NONE
        );
    } */

    /*if (Guntexture)
    {
        SDL_FRect gun = rect;

        gun.w = (float)GunFrameWidth * 2;
        gun.h = (float)GunFrameHeight * 2;

        float cx = rect.x + rect.w * 0.5f;
        float cy = rect.y + rect.h * 0.5f;

        gun.x = cx - gun.w * 0.5f;
        gun.y = cy - gun.h * 0.5f;

        int srcX = GunFrame * GunFrameWidth;

        SDL_FRect src = {
            (float)srcX,
            0.0f,
            (float)GunFrameWidth,
            (float)GunFrameHeight
        };

        float AngleDegrees = angle * 180.0f / 3.1415926535f + 90.0f;

        SDL_RenderTextureRotated(
            renderer,
            Guntexture,
            &src,
            &gun,
            AngleDegrees,
            nullptr,
            SDL_FLIP_NONE
        );
    } */
   
}
