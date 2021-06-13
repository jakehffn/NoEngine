#pragma once

#include <SDL.h>

class Clock {
public:
    Clock();
    double getDeltaTime();
    double getCumulativeTime();
    double tick();
    double getFPS();
    double getAverageFPS();

private:
    Uint64 currentFrame;
    Uint64 lastFrame;

    double deltaTime;
    double cumulativeTime;
    double FPS = 0;
    double previousFPS = 0;
};