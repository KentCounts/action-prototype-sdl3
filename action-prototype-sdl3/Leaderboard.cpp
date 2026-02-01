#include "Leaderboard.h"
#include <algorithm>
#include <fstream>

static const int leftTrim[10] = { 0, 2, 0, 0, 0, 0, 0, 0, -3, -3 };
static const int rightTrim[10] = { -1, 0, 0, 0, 0, 0, 0, 2, 2, 0 };

static void RenderNumberRightAligned(SDL_Renderer* renderer, SDL_Texture* digitsTex,
    int digitW, int digitH, int value,
    float rightX, float y, float scale)
{
    if (!digitsTex || digitW <= 0 || digitH <= 0) return;
    if (value < 0) value = 0;

    std::string s = std::to_string(value);
    float drawH = digitH * scale;

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

bool Leaderboard::Load(const std::string& path)
{
    scores.clear();

    std::ifstream in(path);
    if (!in.is_open()) return false;

    int s;
    while (in >> s)
    {
        if (s >= 0) scores.push_back(s);
    }

    SortAndTrim(10);
    return true;
}

bool Leaderboard::Save(const std::string& path) const
{
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) return false;

    for (int s : scores)
        out << s << "\n";

    return true;
}

void Leaderboard::SortAndTrim(int maxEntries)
{
    std::sort(scores.begin(), scores.end(), std::greater<int>());
    if ((int)scores.size() > maxEntries)
        scores.resize(maxEntries);
}

void Leaderboard::AddScore(int score)
{
    if (score < 0) score = 0;
    scores.push_back(score);
    SortAndTrim(10);
}

void Leaderboard::Render(SDL_Renderer* renderer,
    SDL_Texture* digitsTex,
    int digitW, int digitH,
    int windowWidth, int windowHeight,
    int topN) const
{
    if (!renderer) return;

    // background panel
    SDL_FRect panel = {
        windowWidth * 0.5f - 220.0f,
        140.0f,
        440.0f,
        520.0f
    };

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
    SDL_RenderFillRect(renderer, &panel);

    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderRect(renderer, &panel);

    // rows
    const float startY = panel.y + 40.0f;
    const float rowH = 40.0f;

    // columns: rank right-aligned near left; score right-aligned near right
    const float rankRightX = panel.x + 60.0f;
    const float scoreRightX = panel.x + panel.w - 30.0f;

    int n = (int)scores.size();
    if (topN < n) n = topN;

    for (int i = 0; i < n; ++i)
    {
        float y = startY + i * rowH;

        // rank (1..)
        RenderNumberRightAligned(renderer, digitsTex, digitW, digitH,
            i + 1,
            rankRightX,
            y,
            1.0f);

        // score
        RenderNumberRightAligned(renderer, digitsTex, digitW, digitH,
            scores[i],
            scoreRightX,
            y,
            1.0f);
    }
}
