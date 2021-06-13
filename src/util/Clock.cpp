#include "Clock.h"
#include <iostream>

Clock::Clock() :
    currentFrame{ SDL_GetPerformanceCounter() }, 
    lastFrame{ 0 }, deltaTime{ 0 }, cumulativeTime{ 0 } {}

double Clock::tick() {
    
    lastFrame = currentFrame;
    currentFrame = SDL_GetPerformanceCounter();

    deltaTime = (double)((currentFrame - lastFrame) / (double)SDL_GetPerformanceFrequency());
    cumulativeTime += deltaTime;

    this->previousFPS = FPS;
    this->FPS = 1.0/deltaTime;

    return deltaTime;
}

double Clock::getDeltaTime() {
    return deltaTime;
}

double Clock::getCumulativeTime() {
    return cumulativeTime;
}

double Clock::getFPS() {
    return this->FPS;
}

double Clock::getAverageFPS() {
    return (this->FPS+this->previousFPS)/2.0;
}