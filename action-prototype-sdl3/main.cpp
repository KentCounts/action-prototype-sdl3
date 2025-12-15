#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <SDL3/SDL_keyboard.h>
#include "Player.h"
#include "Bullet.h"
#include <SDL3_image/SDL_image.h>
#include "Enemy.h"

std::vector<Bullet> Bullets;
std::vector<Enemy> Enemies;

bool WasSpaceDown = false;

int main(int argc, char* argv[])
{
    // Initialize SDL Video subsystem
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "SDL3 Test Window",
        800, 600,
        SDL_WINDOW_RESIZABLE
    );
    
  
    // Create a GPU renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Player player(100.0f, 100.0f, 96.0f, 96.0f, 300.0f);
    player.LoadShiptexture(renderer, "assets/ship.png");
    player.LoadEnginetexture(renderer, "assets/engine.png");
    player.LoadGuntexture(renderer, "assets/gun.png");
    Bullet::LoadBullettexture(renderer, "assets/bullet.png");
    Enemy::LoadEnemyTexture(renderer, "assets/asteroid.png");
    

    if (!window) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    Uint64 LastTime = SDL_GetTicks();
    float EnemySpawnTimer = 0.0f;
    const float EnemySpawnInterval = 1.5f; // seconds


    while (running)
    {
        Uint64 Current = SDL_GetTicks();
        float DeltaTime = (Current - LastTime) / 1000.0f;
        LastTime = Current;

        EnemySpawnTimer += DeltaTime;

        if (EnemySpawnTimer >= EnemySpawnInterval)
        {
            EnemySpawnTimer -= EnemySpawnInterval;

            int windowWidth, windowHeight;
            SDL_GetWindowSize(window, &windowWidth, &windowHeight);

            int side = rand() % 4;

            float x, y, vx, vy;
            float angleOffset = ((rand() % 60) - 30) * (3.14159265f / 180.0f);
            float speed = 120.0f;

            switch (side)
            {
            case 0:
                x = -40; y = rand() % windowHeight;
                vx = SDL_cosf(angleOffset) * speed;
                vy = SDL_sinf(angleOffset) * speed;
                break;

            case 1:
                x = windowWidth + 40; y = rand() % windowHeight;
                vx = SDL_cosf(3.14159265f + angleOffset) * speed;
                vy = SDL_sinf(3.14159265f + angleOffset) * speed;
                break;

            case 2:
                x = rand() % windowWidth; y = -40;
                vx = SDL_cosf(1.5707963f + angleOffset) * speed;
                vy = SDL_sinf(1.5707963f + angleOffset) * speed;
                break;

            default:
                x = rand() % windowWidth; y = windowHeight + 40;
                vx = SDL_cosf(-1.5707963f + angleOffset) * speed;
                vy = SDL_sinf(-1.5707963f + angleOffset) * speed;
                break;
            }


            Enemies.emplace_back(x, y, vx, vy, 96.0f);
        }



        // handle all internal events
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            }
        }

        // keyboard stuff
        const bool* keys = SDL_GetKeyboardState(nullptr);

        // Mouse stuff
        float MouseX, MouseY;
        SDL_GetMouseState(&MouseX, &MouseY);

        // firing logic
        bool IsSpacedown = keys[SDL_SCANCODE_SPACE];

        if (IsSpacedown && !WasSpaceDown)
        {
            SDL_FPoint gunPos;

            if (player.FireLeftNext)
                gunPos = player.GetLeftGunPos();
            else
                gunPos = player.GetRightGunPos();

            player.FireLeftNext = !player.FireLeftNext;


            // direction toward mouse
            float DirectionX = MouseX - gunPos.x;
            float DirectionY = MouseY - gunPos.y;

            Bullets.emplace_back(gunPos.x, gunPos.y, DirectionX, DirectionY, 700.0f);
        }

        WasSpaceDown = IsSpacedown;

        

        player.SetFiring(keys[SDL_SCANCODE_SPACE]);

        // render loop
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        // update player
        player.update(DeltaTime, windowWidth, windowHeight, MouseX, MouseY);

        //update bullets
        for (auto& b : Bullets)
        {
            b.update(DeltaTime);
        }

        // update enemies
        for (auto& e : Enemies)
        {
            e.update(DeltaTime);
        }

        // remove enemies off screen
        Enemies.erase(
            std::remove_if(Enemies.begin(), Enemies.end(),
                [&](const Enemy& e) { return e.isOffScreen(windowWidth, windowHeight); }),
            Enemies.end()
        );


        // remove bullets off-screen
        Bullets.erase(
            std::remove_if(Bullets.begin(), Bullets.end(),
                [&](const Bullet& b) {return b.isOffScreen(windowWidth, windowHeight); }),
            Bullets.end()
        );
        
        // black screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // player rendering
        player.render(renderer);

        // bullet render
        for (auto& b : Bullets)
        {
            b.render(renderer);
        }

        // enemy render
        for (const auto& e : Enemies)
        {
            e.render(renderer);
        }


        // score board render
        SDL_FRect Scoreboard = { windowWidth - 150.0f, 20.0f, 120.0f, 40.0f  };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 200);
        SDL_RenderFillRect(renderer, &Scoreboard);

        // Present the window thing?
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
