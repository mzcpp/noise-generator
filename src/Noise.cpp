#include <SDL2/SDL.h>

#include "Noise.hpp"
#include "Game.hpp"

#include <iostream>
#include <cstdint>
#include <ctime>
#include <random>

Noise::Noise(Game* game, int texture_width, int texture_height) : 
    game_(game), 
    width_(texture_width), 
    height_(texture_height), 
    pixels_(nullptr), 
    texture_(nullptr)
{
    // SetSeed(398341902);
    SetRandomSeed();
    
    pixels_ = new Uint32[width_ * height_];
    texture_ = SDL_CreateTexture(game_->renderer_, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width_, height_);
    
    SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_NONE);

    memset(pixels_, 0, width_ * height_ * sizeof(Uint32));

    SDL_UpdateTexture(texture_, NULL, pixels_, width_ * sizeof(Uint32));
}

Noise::~Noise()
{
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;

    delete[] pixels_;
    pixels_ = nullptr;
}

void Noise::Tick()
{

}

void Noise::Render()
{
    SDL_RenderCopy(game_->renderer_, texture_, NULL, NULL);
}

Uint32 Noise::GetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
	return (b << 24) + (g << 16) + (r << 8) + a;
	#else
	return (a << 24) + (r << 16) + (g << 8) + b;
	#endif
}

double Noise::Fade(double n)
{
    return n * n * n * (n * (n * 6 - 15) + 10);
}

double Noise::Lerp(double a, double b, double n)
{
    return a + n * (b - a);
}

std::uint32_t Noise::Hash(std::uint32_t a)
{
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

std::uint64_t Noise::SaltHash(std::uint64_t x, std::uint64_t salt)
{
    const std::uint64_t m = 0x5bd1e995U;
    std::uint64_t hash = salt;
    std::uint64_t k = x;
    k *= m;
    k ^= k >> 24;
    k *= m;
    hash *= m;
    hash ^= k;
    hash ^= hash >> 13;
    hash *= m;
    hash ^= hash >> 15;
    return hash;
}

void Noise::SetSeed(long long int new_seed)
{
    seed_ = new_seed;
    rng_64_.seed(new_seed);
}

void Noise::SetRandomSeed()
{
    std::random_device rd;
    seed_ = rd();
    rng_64_.seed(seed_);
}