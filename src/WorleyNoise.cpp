#include "Noise.hpp"
#include "WorleyNoise.hpp"
#include "Constants.hpp"

#include <random>
#include <iostream>
#include <cmath>

class Game;

WorleyNoise::WorleyNoise(Game* game, int texture_width, int texture_height) : 
    Noise(game, texture_width, texture_height)
{
}

void WorleyNoise::Generate1D()
{
    
}

void WorleyNoise::Generate2D()
{
    const double grid_size = 100.0;

    for (std::size_t i = 0; i < static_cast<std::size_t>(width_ * height_); ++i)
    {
        const int x = i % width_;
        const int y = i / width_;

        const int color = std::clamp(static_cast<int>(Worley(x, y, grid_size)), 0x0, 0xFF);

        pixels_[i] = GetColor(color, color, color, 0xff);
    }

    SDL_UpdateTexture(texture_, NULL, pixels_, width_ * sizeof(Uint32));
}

double WorleyNoise::Worley(int x, int y, double grid_size)
{
    const int x_prime = 37;
    const int y_prime = 41;

    const double dx = static_cast<double>(x) / grid_size; 
    const double dy = static_cast<double>(y) / grid_size;

    const int xi = std::floor(dx);
    const int yi = std::floor(dy);

    double min_distance = std::numeric_limits<double>::max();

    for (int yo = -1; yo < 2; ++yo)
    {
        for (int xo = -1; xo < 2; ++xo)
        {
            if ((xi + xo) < 0 || (yi + yo) < 0 || (xi + xo) >= constants::screen_width / grid_size || (yi + yo) >= constants::screen_height / grid_size)
            {
                continue;
            }

            const SDL_Point random_cell_point = GetRandomCellPoint(xi, xo, yi, yo, x_prime, y_prime, seed_, grid_size);
            // pixels_[random_cell_point.y * width_ + random_cell_point.x] = GetColor(255, 0, 255, 0xff);

            const double distance = std::sqrt(((random_cell_point.x - x) * (random_cell_point.x - x)) + ((random_cell_point.y - y) * (random_cell_point.y - y)));
            min_distance = std::min(min_distance, distance);
        }
    }

    return min_distance;
}

SDL_Point WorleyNoise::GetRandomCellPoint(int xi, int xo, int yi, int yo, int x_prime, int y_prime, int seed, double grid_size)
{
    SDL_Point result_point;

    const int xio = (xi + xo);
    const int yio = (yi + yo);

    result_point.x = (SaltHash(((xio + 1) * x_prime) + xio + yio, seed) % static_cast<int>(grid_size)) + (xio * grid_size);
    result_point.y = (SaltHash(((yio + 1) * y_prime) + xio - yio, seed) % static_cast<int>(grid_size)) + (yio * grid_size);
    
    return result_point;
}
