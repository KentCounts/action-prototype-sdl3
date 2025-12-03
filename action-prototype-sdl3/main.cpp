#include <SDL3/SDL.h>
#include <iostream>

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

    if (!window) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Keep window open for 5 seconds
    SDL_Delay(5000);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
