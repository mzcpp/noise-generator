#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>

#include "Noise.hpp"

#include <vector>
#include <memory>

class Game
{
private:
    bool initialized_;
    bool running_;
    int ticks_;
    bool shift_pressed_;

    SDL_Point mouse_position_;

    std::unique_ptr<Noise> noise_;

public:
    SDL_Window* window_;
    SDL_Renderer* renderer_;

    Game();
    
    ~Game();

    bool Initialize();

    void Finalize();

    void Run();

    void HandleEvents();

    void Tick();

    void Render();
};

#endif
