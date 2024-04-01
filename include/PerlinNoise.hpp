#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include <SDL2/SDL.h>

#include "Noise.hpp"

#include <array>
#include <random>

class PerlinNoise : public Noise
{
private:
    std::array<int, 512> permutation_table_;

public:
    PerlinNoise(Game* game, int texture_width, int texture_height);

    void Generate1D() override;
    
    void Generate2D() override;

    double Perlin(double x);
    
    double Perlin(double x, double y);
    
    double Perlin(double x, double y, double z);

    double DotGrad(int hash, double x);
    
    double DotGrad(int hash, double x, double y);
    
    double DotGrad(int hash, double x, double y, double z);
};

#endif