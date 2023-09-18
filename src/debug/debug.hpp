#pragma once

#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"

#include "game.hpp"

#include "debug_timer.hpp"

#include "render_collision.hpp"

class DebugWindow {
public:
    DebugWindow() = default;
    DebugWindow(Game*);
    void show();
private:
    void showMenuBar();

    void showTextureAtlas();
    void showEntityViewer();
    void showShaderViewer();

    void removeSelectedEntity();

    Game* game;

    // Texture atlas
    bool open_texture_atlas = false;

    // Entity Viewer
    bool open_entity_viewer = false;
    entt::entity selected_entity = entt::null;

    // Shader Viewer
    bool open_shader_viewer = false;
    ShaderProgram* selected_shader = NULL;
    bool auto_scroll{true};
    bool scroll_to_botton{false};

    // Collision
    bool show_collision_boxes{false};
};