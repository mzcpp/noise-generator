#include "Noise.hpp"
#include "WhiteNoise.hpp"

#include <random>

class Game;

WhiteNoise::WhiteNoise(Game* game, int texture_width, int texture_height) : Noise(game, texture_width, texture_height)
{
    Generate2D();
}

void WhiteNoise::Generate1D()
{
}

void WhiteNoise::Generate2D()
{
    //const Uint32 color = GetColor(std::rand() % 0xff, std::rand() % 0xff, std::rand() % 0xff, 0xff);

    for (std::size_t i = 0; i < static_cast<std::size_t>(width_ * height_); ++i)
    {
        // pixels_[i] = GetColor(std::rand() % 0xff, std::rand() % 0xff, std::rand() % 0xff, 0xff);

        const int col = std::rand() % 256;
        pixels_[i] = GetColor(col, col, col, 0xff);
    }

    SDL_UpdateTexture(texture_, NULL, pixels_, width_ * sizeof(Uint32));
}