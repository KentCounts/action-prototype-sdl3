#pragma once
#include <vector>
#include <string>
#include <SDL3/SDL.h>

class Leaderboard
{
public:
    bool Load(const std::string& path);
    bool Save(const std::string& path) const;

    void AddScore(int score);
    const std::vector<int>& GetScores() const { return scores; }

    void Render(SDL_Renderer* renderer,
        SDL_Texture* digitsTex,
        int digitW, int digitH,
        int windowWidth, int windowHeight,
        int topN = 10) const;

private:
    void SortAndTrim(int maxEntries);

    std::vector<int> scores;
};