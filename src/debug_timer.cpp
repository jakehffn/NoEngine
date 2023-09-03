#include "debug_timer.hpp"

#include <iostream>

bool DebugTimer::open_timers_window = false;
DebugTimer* DebugTimer::prev_timer = NULL;

DebugTimer::DebugTimer(const char* name) : name{name}, higher_level_timer{DebugTimer::prev_timer} {
    DebugTimer::prev_timer = this;

    this->timer_started = DebugTimer::open_timers_window;
    if (DebugTimer::open_timers_window) {
        this->start = SDL_GetPerformanceCounter();
        ImGui::SetNextWindowSize({500, 440}, 0);
        if (ImGui::Begin("Timers", &DebugTimer::open_timers_window, 
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize
        )) {
            if (this->higher_level_timer == 0 || this->higher_level_timer->not_collapsed) {
                this->not_collapsed = ImGui::CollapsingHeader(this->name);
            }
            ImGui::Indent();
        }
        ImGui::End();
    }
}

DebugTimer::~DebugTimer() {
    DebugTimer::prev_timer = this->higher_level_timer;
    
    if (DebugTimer::open_timers_window && this->timer_started) {
        auto duration = (double)(SDL_GetPerformanceCounter() - this->start)/SDL_GetPerformanceFrequency()*1000.0;
        ImGui::SetNextWindowSize({500, 440}, 0);
        if (ImGui::Begin("Timers", &DebugTimer::open_timers_window, 
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize
        )) {
            if ((this->higher_level_timer == 0 || this->higher_level_timer->not_collapsed) && this->not_collapsed) {
                ImGui::Text("%.2fms", duration);
            }
            ImGui::Unindent();
        }
        ImGui::End();
    }
}