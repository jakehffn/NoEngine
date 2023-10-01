#pragma once

#include <iostream>
#include <unordered_set>
#include <array>
#include <cassert>

#include <entt/entt.hpp>
#include <SDL.h>
#ifndef NDEBUG
    #include "imgui_impl_opengl3.h"
    #include "imgui_impl_sdl2.h"
#endif

#include "system.hpp"
#include "clock.hpp"

#include "globals.hpp"

#include "debug_timer.hpp"

class Input {
public:
    Input();
    void update();

    bool isActive(SDL_Keycode key);
    bool isAdded(SDL_Keycode key);
    bool isRemoved(SDL_Keycode key);
    bool isQuit();

    int getMouseX();
    int getMouseY();
    bool isMouseActive(Uint8 mouse_button);
    bool isMouseAdded(Uint8 mouse_button);
    bool isMouseRemoved(Uint8 mouse_button);

    bool interactionEnabled();
    void disableInteraction();
    void enableInteraction();

private:

    int mouse_x{0};
    int mouse_y{0};

    bool quit{false};

    std::unordered_set<SDL_Keycode> active; 
    std::unordered_set<SDL_Keycode> added; 
    std::unordered_set<SDL_Keycode> removed; 

    std::array<bool, 256> mouse_active; 

    std::array<bool, 256> mouse_added; 
    std::array<bool, 256> mouse_added_reset; 
    unsigned int mouse_added_reset_num{0};

    std::array<bool, 256> mouse_removed; 
    std::array<bool, 256> mouse_removed_reset; 
    unsigned int mouse_removed_reset_num{0};

    bool interaction_enabled{true};
};