#include <iostream>

#include "clock.hpp"

Clock::Clock() :
    current_frame{ SDL_GetPerformanceCounter() }, 
    last_frame{ 0 }, delta_time{ 0 }, cumulative_time{ 0 }, smoothed_fps{144.0} {}

double Clock::tick() {
    
    last_frame = current_frame;
    current_frame = SDL_GetPerformanceCounter();

    delta_time = (double)((current_frame - last_frame) / (double)SDL_GetPerformanceFrequency() *1000);
    cumulative_time += delta_time;

    this->fps = 1.0/delta_time * 1000;

    double smoothing = 0.99;

    this->smoothed_fps = (smoothed_fps * smoothing) + (this->fps * (1 - smoothing));

    return delta_time;
}

// Returns the time change since the last tick in milliseconds
double Clock::getDeltaTime() {
    return delta_time;
}

double Clock::getCumulativeTime() {
    return cumulative_time;
}

double Clock::getFPS() {
    return this->fps;
}

double Clock::getSmoothedFPS() {
    return this->smoothed_fps;
}