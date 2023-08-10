#pragma once

#include <SDL.h>

class Clock {
public:
    Clock();
    double getDeltaTime();
    double getCumulativeTime();
    double tick();
    double getFPS();
    double getSmoothedFPS();

private:
    Uint64 current_frame;
    Uint64 last_frame;

    double delta_time;
    double cumulative_time;
    double fps{0};
    double smoothed_fps{0};
};