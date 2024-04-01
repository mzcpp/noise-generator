#ifndef RANDOM_NOISE_HPP
#define RANDOM_NOISE_HPP

#include <SDL2/SDL.h>

#include "Noise.hpp"

class WhiteNoise : public Noise
{
public:
    WhiteNoise(Game* game, int texture_width, int texture_height);

    void Generate1D() override;

    void Generate2D() override;
};

#endif