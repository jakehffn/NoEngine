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
    void showEntityViewer();

    Game* game;

    // Texture atlas
    bool openTextureAtlas = false;

    // Entity Viewer
    bool openEntityViewer = false;
    entt::entity selected_entity;
};