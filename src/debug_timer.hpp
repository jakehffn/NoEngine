#pragma once

#ifndef NDEBUG

#include <SDL.h>

#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"

class DebugTimer {
public:
    DebugTimer(const char* name);
    ~DebugTimer();

    static bool open_timers_window;
    static DebugTimer* prev_timer;
private:
    Uint64 start;
    const char* name;
    bool not_collapsed{false};
    bool timer_started{false};
    DebugTimer* higher_level_timer;
};

#define DEBUG_TIMER(variable_name, name) auto variable_name{DebugTimer(name)}

#else

#define DEBUG_TIMER(variable_name, name)

#endif