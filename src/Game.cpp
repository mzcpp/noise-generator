#include "Constants.hpp"
#include "Noise.hpp"
#include "Game.hpp"
#include "WhiteNoise.hpp"
#include "PerlinNoise.hpp"
#include "WorleyNoise.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <cstdint>

Game::Game() : 
    initialized_(false), 
    running_(false), 
    ticks_(0), 
    shift_pressed_(false), 
    noise_(nullptr), 
    window_(nullptr), 
    renderer_(nullptr)
{
    initialized_ = Initialize();

    std::srand(std::time(nullptr));
    std::rand();
}

Game::~Game()
{
    Finalize();
}

bool Game::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not be initialized! SDL Error: %s\n", SDL_GetError());
		return false;
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
    {
		printf("%s\n", "Warning: Texture filtering is not enabled!");
    }

    window_ = SDL_CreateWindow(constants::game_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, constants::screen_width, constants::screen_height, SDL_WINDOW_SHOWN);

    if (window_ == nullptr)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

    if (renderer_ == nullptr)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
    }
    
    constexpr int img_flags = IMG_INIT_PNG;

    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

    if (!(IMG_Init(img_flags) & img_flags))
    {
        printf("SDL_image could not be initialized! SDL_image Error: %s\n", IMG_GetError());
		return false;
    }

    return true;
}

void Game::Finalize()
{
    SDL_DestroyWindow(window_);
    window_ = nullptr;

    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;

    SDL_Quit();
    IMG_Quit();
}

void Game::Run()
{
    if (!initialized_)
	{
		printf("%s\n", "Game has not been initialized!");
		return;
	}

	running_ = true;

    constexpr double ms = 1.0 / 60.0;
    std::uint64_t last_time = SDL_GetPerformanceCounter();
    long double delta = 0.0;

    double timer = SDL_GetTicks();
    int frames = 0;
    int ticks = 0;

    while (running_)
    {
        const std::uint64_t now = SDL_GetPerformanceCounter();
        const long double elapsed = static_cast<long double>(now - last_time) / static_cast<long double>(SDL_GetPerformanceFrequency());

        last_time = now;
        delta += elapsed;

        HandleEvents();

        while (delta >= ms)
        {
            Tick();
            delta -= ms;
            ++ticks;
        }

		//printf("%Lf\n", delta / ms);
        Render();
        ++frames;

        if (SDL_GetTicks() - timer > 1000.0)
        {
			//printf("Frames: %d, Ticks: %d\n", frames, ticks);
            timer += 1000.0;
            frames = 0;
            ticks = 0;
        }
    }
}

void Game::HandleEvents()
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        SDL_GetMouseState(&mouse_position_.x, &mouse_position_.y);
        
        if (e.type == SDL_QUIT)
        {
            running_ = false;
            return;
        }

        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_LSHIFT)
            {
                shift_pressed_ = true;
            }

            if (e.key.keysym.sym == SDLK_1)
            {
                noise_ = std::make_unique<WhiteNoise>(this, constants::screen_width, constants::screen_height);
            }
            else if (e.key.keysym.sym == SDLK_2)
            {
                if (noise_ != nullptr && dynamic_cast<PerlinNoise*>(noise_.get()) != nullptr)
                {
                    noise_->SetRandomSeed();
                    // noise_->SetSeed(398341902);
                }
                else
                {
                    noise_ = std::make_unique<PerlinNoise>(this, constants::screen_width, constants::screen_height);
                }
                
                shift_pressed_ ? noise_->Generate1D() : noise_->Generate2D();
            }
            else if (e.key.keysym.sym == SDLK_3)
            {
                if (noise_ != nullptr && dynamic_cast<WorleyNoise*>(noise_.get()) != nullptr)
                {
                    noise_->SetRandomSeed();
                    // noise_->SetSeed(398341902);
                }
                else
                {
                    noise_ = std::make_unique<WorleyNoise>(this, constants::screen_width, constants::screen_height);
                }

                noise_->Generate2D();
            }
            else if (e.key.keysym.sym == SDLK_r && noise_ != nullptr)
            {
                noise_.reset(nullptr);
            }
        }
        else if (e.type == SDL_KEYUP)
        {
            if (e.key.keysym.sym == SDLK_LSHIFT)
            {
                shift_pressed_ = false;
            }
        }
    }
}

void Game::Tick()
{
    ++ticks_;

    if (noise_ != nullptr)
    {
        noise_->Tick();
    }
}

void Game::Render()
{
    SDL_RenderSetViewport(renderer_, NULL);
    SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0xff, 0xff);
    SDL_RenderClear(renderer_);

    if (noise_ != nullptr)
    {
        noise_->Render();
    }
    
    SDL_RenderPresent(renderer_);
}