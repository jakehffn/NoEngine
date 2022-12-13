#pragma once

#include <iostream>

#include <entt/entt.hpp>

#include "System.h"
#include "Clock.h"

#include <unordered_set>
#include <cassert>
#include <SDL.h>

#include "consts.h"


class InputManager {
public:
    InputManager();
    void update();

    bool isActive(SDL_Keycode key);
    bool isAdded(SDL_Keycode key);
    bool isRemoved(SDL_Keycode key);
    bool isQuit();

    int getMouseX();
    int getMouseY();

private:

    int mouseX;
    int mouseY;

    SDL_Event e;
    bool quit;

    std::unordered_set<SDL_Keycode> active; 
    std::unordered_set<SDL_Keycode> added; 
    std::unordered_set<SDL_Keycode> removed; 
};