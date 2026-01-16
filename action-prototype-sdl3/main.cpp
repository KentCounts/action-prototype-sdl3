#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <string>
#include <SDL3/SDL_keyboard.h>
#include "Player.h"
#include "Bullet.h"
#include <SDL3_image/SDL_image.h>
#include "Enemy.h"
#include "BackgroundLayer.h"

std::vector<Bullet> Bullets;
std::vector<Enemy> Enemies;

BackgroundLayer bg1, bg2, bg3;

int Score = 0;

bool WasSpaceDown = false;

enum class GameState
{
    Title,
    Playing
};

GameState State = GameState::Title;

static bool PointInRect(float px, float py, const SDL_FRect& r)
{
    return px >= r.x && px <= (r.x + r.w) &&
        py >= r.y && py <= (r.y + r.h);
}

// png texture loader 
static SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* path)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        std::cout << "IMG_Load failed: " << path << " error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);

    if (!tex) {
        std::cout << "CreateTexture failed: " << path << " error: " << SDL_GetError() << std::endl;
    }
    return tex;
}

// button texture renderer helper
static void RenderCentered(SDL_Renderer* renderer, SDL_Texture* tex, const SDL_FRect& r)
{
    if (!tex) return;

    float tw, th;
    SDL_GetTextureSize(tex, &tw, &th);

    SDL_FRect dst = {
        r.x + (r.w - tw) * 0.5f,
        r.y + (r.h - th) * 0.5f,
        tw,
        th
    };

    SDL_RenderTexture(renderer, tex, nullptr, &dst);
}

static const int leftTrim[10] = { 0, 2, 0, 0, 0, 0, 0, 0, -3, -3 };
static const int rightTrim[10] = { -1, 0, 0, 0, 0, 0, 0, 2, 2, 0 };

// digit texture helper
static void RenderNumberRightAligned(SDL_Renderer* renderer, SDL_Texture* digitsTex,
    int digitW, int digitH, int value,
    float rightX, float y, float scale)
{
    if (!digitsTex || digitW <= 0 || digitH <= 0) return;
    if (value < 0) value = 0;

    std::string s = std::to_string(value);

    float drawH = digitH * scale;

    // Compute total width using TRIMMED widths so right-align is correct
    float totalW = 0.0f;
    for (char c : s)
    {
        int d = c - '0';
        int sw = digitW - leftTrim[d] - rightTrim[d];
        totalW += sw * scale;
    }

    float x = rightX - totalW;

    for (char c : s)
    {
        int d = c - '0';

        int sx = d * digitW + leftTrim[d];
        int sw = digitW - leftTrim[d] - rightTrim[d];

        SDL_FRect src = { (float)sx, 0.0f, (float)sw, (float)digitH };
        SDL_FRect dst = { x, y, sw * scale, drawH };

        SDL_RenderTexture(renderer, digitsTex, &src, &dst);
        x += dst.w;
    }
}



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
        1280, 720,
        0
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
    // player.LoadEnginetexture(renderer, "assets/engine.png");
    // player.LoadGuntexture(renderer, "assets/gun.png");
    // player.LoadFlametexture(renderer, "assets/flame.png");

    // Bullet::LoadBullettexture(renderer, "assets/bullet.png");
    Enemy::LoadEnemyTexture(renderer, "assets/asteroid.png");

    SDL_Texture* titleTex = LoadTexture(renderer, "assets/title.png");
    SDL_Texture* newGameTex = LoadTexture(renderer, "assets/new_game.png");
    SDL_Texture* leaderTex = LoadTexture(renderer, "assets/leaderboards.png");
    SDL_Texture* quitTex = LoadTexture(renderer, "assets/quit.png");

    bg1.Load(renderer, "assets/background 1.png", 640, 360, 9);
    bg2.Load(renderer, "assets/background 2.png", 640, 360, 9);
    bg3.Load(renderer, "assets/background 3.png", 640, 360, 9);

    bg1.SetFrameTime(0.10f);
    bg2.SetFrameTime(0.08f);
    bg3.SetFrameTime(0.06f);

    SDL_Texture* digitsTex = LoadTexture(renderer, "assets/digits.png");
    const int digitCount = 10;
    int digitW = 0, digitH = 0;

    if (digitsTex)
    {
        float tw, th;
        SDL_GetTextureSize(digitsTex, &tw, &th);
        digitW = (int)(tw / digitCount);
        digitH = (int)(th);
    }


    

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

        bg1.Update(DeltaTime);
        bg2.Update(DeltaTime);
        bg3.Update(DeltaTime);

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

        // menu buttons
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        SDL_FRect NewGameButton = { windowWidth * 0.5f - 150.0f, 300.0f, 300.0f, 60.0f };
        SDL_FRect LeaderButton = { windowWidth * 0.5f - 150.0f, 380.0f, 300.0f, 60.0f };
        SDL_FRect QuitButton = { windowWidth * 0.5f - 150.0f, 460.0f, 300.0f, 60.0f };



        // handle all internal events
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (State == GameState::Title && event.button.button == SDL_BUTTON_LEFT)
                {
                    float mx = event.button.x;
                    float my = event.button.y;

                    if (PointInRect(mx, my, NewGameButton))
                    {
                        // Reset game state
                        Bullets.clear();
                        Enemies.clear();
                        EnemySpawnTimer = 0.0f;

                        // Put player back where you want
                        // (If your Player class has a setter, use it; otherwise reconstruct)
                        // Example: player.SetPosition(100,100);

                        State = GameState::Playing;
                        Score = 0;
                    }
                    else if (PointInRect(mx, my, LeaderButton))
                    {
                        // placeholder: do nothing for now
                    }
                    else if (PointInRect(mx, my, QuitButton))
                    {
                        running = false;
                    }
                }
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
            /*SDL_FPoint gunPos;

            if (player.FireLeftNext)
                gunPos = player.GetLeftGunPos();
            else
                gunPos = player.GetRightGunPos();

            player.FireLeftNext = !player.FireLeftNext;*/

            float PlayerX = player.getXCenter();
            float PlayerY = player.getYCenter();

            float DirectionX = MouseX - PlayerX;
            float DirectionY = MouseY - PlayerY;

            Bullets.emplace_back(PlayerX, PlayerY, DirectionX, DirectionY, 700.0f);
        } 

        WasSpaceDown = IsSpacedown;

        

        // player.SetFiring(keys[SDL_SCANCODE_SPACE]);

        // render loop
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        // black screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        bg1.Render(renderer, windowWidth, windowHeight);
        bg2.Render(renderer, windowWidth, windowHeight);
        bg3.Render(renderer, windowWidth, windowHeight);


        if (State == GameState::Playing)
        {
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

            auto CircleHit = [](SDL_FPoint a, float ar, SDL_FPoint b, float br) -> bool
                {
                    float dx = a.x - b.x;
                    float dy = a.y - b.y;
                    float r = ar + br;
                    return (dx * dx + dy * dy) <= (r * r);
                };

            // Bullet Enemy collision
            for (size_t bi = 0; bi < Bullets.size(); )
            {
                bool bulletRemoved = false;

                for (size_t ei = 0; ei < Enemies.size(); )
                {
                    if (CircleHit(Bullets[bi].GetPos(), Bullets[bi].GetRadius(),
                        Enemies[ei].GetPos(), Enemies[ei].GetRadius()))
                    {

                        Bullets[bi] = Bullets.back();
                        Bullets.pop_back();


                        Enemies[ei] = Enemies.back();
                        Enemies.pop_back();

                        Score += 100;

                        bulletRemoved = true;
                        break;
                    }
                    else
                    {
                        ++ei;
                    }
                }

                if (!bulletRemoved)
                    ++bi;
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
            SDL_FRect Scoreboard = { windowWidth - 150.0f, 20.0f, 120.0f, 40.0f };
            // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 120);
            // SDL_RenderFillRect(renderer, &Scoreboard);


            // Score digits
            RenderNumberRightAligned(renderer, digitsTex, digitW, digitH,
                Score,
                Scoreboard.x + Scoreboard.w - 15.0f,  
                Scoreboard.y + 15.0f,                 
                1.0f);                                
        }

        else
        {

            // Draw buttons
            float mx, my;
            SDL_GetMouseState(&mx, &my);

            auto DrawButton = [&](const SDL_FRect& r)
                {
                    bool hover = PointInRect(mx, my, r);
                    if (hover) SDL_SetRenderDrawColor(renderer, 90, 90, 90, 220);
                    else       SDL_SetRenderDrawColor(renderer, 60, 60, 60, 220);

                    SDL_RenderFillRect(renderer, &r);
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                    SDL_RenderRect(renderer, &r);
                };

            DrawButton(NewGameButton);
            DrawButton(LeaderButton);
            DrawButton(QuitButton);

            SDL_FRect titleDst = { windowWidth * 0.5f - 300.0f, 140.0f, 600.0f, 120.0f };
            SDL_RenderTexture(renderer, titleTex, nullptr, &titleDst);

            RenderCentered(renderer, newGameTex, NewGameButton);
            RenderCentered(renderer, leaderTex, LeaderButton);
            RenderCentered(renderer, quitTex, QuitButton);

        }


        // Present the window thing?
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
