#include <SDL3/SDL.h>
#include <iostream>
#include "Player.h"

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

    Player player(100.0f, 100.0f, 50.0f, 50.0f, 300.0f);


    if (!window) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    Uint64 LastTime = SDL_GetTicks();

    while (running)
    {
        Uint64 Current = SDL_GetTicks();
        float DeltaTime = (Current - LastTime) / 1000.0f;
        LastTime = Current;

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

        // render loop
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        player.update(DeltaTime, windowWidth, windowHeight);
        
        // black screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // player rendering
        player.render(renderer);

        // Present the window thing?
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
