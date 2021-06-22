#pragma once

#include <unordered_set>
#include <cassert>
#include <SDL.h>

#include "consts.h"

class Input {
public:
    Input();
    ~Input();

    void update();

    void setToggle(SDL_Keycode toggle);
    void setAbridge(SDL_Keycode abridge);

    bool isKeyDown(SDL_Keycode key);
    int getMouseX();
    int getMouseY();
    bool quitProgram();

private:
    std::unordered_set<SDL_Keycode> keyInputs;
    int mouseX;
    int mouseY;

    std::unordered_set<SDL_Keycode> toggleInputs; 
    std::unordered_set<SDL_Keycode> abridgedInputs;

    SDL_Event e;
    bool quit;
};