#ifndef NOISE_HPP
#define NOISE_HPP

#include <SDL2/SDL.h>

#include <random>

class Game;

class Noise
{
protected:
    Game* game_;
    
    int width_;
    int height_;

    std::mt19937_64 rng_64_;
    long long int seed_;
    
    Uint32* pixels_;
    SDL_Texture* texture_;

    Noise(Game* game, int texture_width, int texture_height);

public:
    virtual ~Noise();

    void Tick();

    void Render();

    Uint32 GetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    double Fade(double n);

    double Lerp(double a, double b, double n);

    virtual void Generate1D() = 0;
    
    virtual void Generate2D() = 0;

    std::uint32_t Hash(std::uint32_t a);

    std::uint64_t SaltHash(std::uint64_t x, std::uint64_t salt);

    void SetSeed(long long int new_seed);

    void SetRandomSeed();
};

#endif