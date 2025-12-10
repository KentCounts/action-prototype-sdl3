#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <SDL3/SDL_keyboard.h>
#include "Player.h"
#include "Bullet.h"
#include <SDL3_image/SDL_image.h>

std::vector<Bullet> Bullets;


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

    if (!window) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    Uint64 LastTime = SDL_GetTicks();

    // bullet cool down
    float fireCooldown = 0.0f;
    const float fireRate = 1.0f / 3.0f;


    while (running)
    {
        Uint64 Current = SDL_GetTicks();
        float DeltaTime = (Current - LastTime) / 1000.0f;
        LastTime = Current;
        if (fireCooldown > 0)
        {
            fireCooldown -= DeltaTime;
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

        if (keys[SDL_SCANCODE_SPACE] && fireCooldown <= 0.0f)
        {

            // player position
            float PlayerX = player.getXCenter();
            float PlayerY = player.getYCenter();

            //direction towards mouse
            float DirectionX = MouseX - PlayerX;
            float DirectionY = MouseY - PlayerY;

            Bullets.emplace_back(PlayerX, PlayerY, DirectionX, DirectionY, 500.0f);

            fireCooldown = fireRate;
        }

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
            b.render(renderer);



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
