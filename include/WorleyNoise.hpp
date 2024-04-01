#ifndef VORONOI_NOISE_HPP
#define VORONOI_NOISE_HPP

#include <SDL2/SDL.h>

#include "Noise.hpp"

class WorleyNoise : public Noise
{
public:
    WorleyNoise(Game* game, int texture_width, int texture_height);

    void Generate1D() override;

    void Generate2D() override;

    double Worley(int x, int y, double grid_size);

    SDL_Point GetRandomCellPoint(int xi, int xo, int yi, int yo, int x_prime, int y_prime, int seed, double grid_size);
};

#endif