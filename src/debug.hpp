#pragma once

#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"

#include "game.hpp"

class DebugWindow {
public:
    DebugWindow() = default;
    DebugWindow(Game*);
    void show();
private:
    void showMenuBar();
    void showTextureAtlas();

    Game* game;
    bool openTextureAtlas = false;
};