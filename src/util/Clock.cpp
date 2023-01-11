#include "Clock.h"
#include <iostream>

Clock::Clock() :
    currentFrame{ SDL_GetPerformanceCounter() }, 
    lastFrame{ 0 }, deltaTime{ 0 }, cumulativeTime{ 0 }, smoothedFPS{144.0} {}

double Clock::tick() {
    
    lastFrame = currentFrame;
    currentFrame = SDL_GetPerformanceCounter();

    deltaTime = (double)((currentFrame - lastFrame) / (double)SDL_GetPerformanceFrequency() *1000);
    cumulativeTime += deltaTime;

    this->FPS = 1.0/deltaTime;

    double smoothing = 0.99;

    this->smoothedFPS = (smoothedFPS * smoothing) + (this->FPS * (1 - smoothing));

    return deltaTime;
}

// Returns the time change since the last tick in milliseconds
double Clock::getDeltaTime() {
    return deltaTime;
}

double Clock::getCumulativeTime() {
    return cumulativeTime;
}

double Clock::getFPS() {
    return this->FPS;
}

double Clock::getSmoothedFPS() {
    return this->smoothedFPS;
}