#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <limits>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <entt\entt.hpp>
#ifndef NDEBUG
    #include "imgui_impl_opengl3.h"
    #include "imgui_impl_sdl2.h"
#endif

#include "clock.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "texture_atlas.hpp"
#include "sprite_sheet_atlas.hpp"
#include "shader_manager.hpp"
#include "component_grid.hpp"
#include "renderable.hpp"
#include "collision.hpp"

#include "input_system.hpp"
#include "render_system.hpp"
#include "collision_system.hpp"
#include "movement_system.hpp"
#include "animation_system.hpp"
#include "camera_system.hpp"
#include "gui_system.hpp"
#include "map_loader_system.hpp"
#include "text_system.hpp"
#include "state_machine_system.hpp"

#include "debug_timer.hpp"

struct Game {
    Game(SDL_Window* window);

    void mainLoop(void (*debugCallback)());

    inline void startFrame();
    inline void endFrame();

    SDL_Window* window;
    entt::registry registry;
    std::vector<System*> systems;

    Clock clock = Clock();
    Camera world_camera = Camera();
    Camera gui_camera = Camera();
    Input input_manager = Input();
    // This should be read from the map at some point
    TextureAtlas texture_atlas;
    SpriteSheetAtlas sprite_sheet_atlas;
    ShaderManager shader_manager{ShaderManager(registry)};

    GLuint screen_texture;

    ComponentGrid<Renderable> renderable_grid = ComponentGrid<Renderable>(
        this->registry, [](entt::registry& registry, entt::entity entity) {
            auto& spacial = registry.get<Spacial>(entity);
            lightgrid::bounds bounds{
                static_cast<int>(spacial.pos.x), static_cast<int>(spacial.pos.y), 
                static_cast<int>(spacial.dim.x), static_cast<int>(spacial.dim.y) 
            };
            return bounds;
        }
    );
    ComponentGrid<Collision> collision_grid = ComponentGrid<Collision>(
        this->registry, [](entt::registry& registry, entt::entity entity) {
            auto spacial = registry.get<Spacial>(entity);
            auto collision = registry.get<Collision>(entity);

            float min_x{std::numeric_limits<float>::max()};
            float max_x{std::numeric_limits<float>::min()};
            float min_y{std::numeric_limits<float>::max()};
            float max_y{std::numeric_limits<float>::min()};

            for (auto bounding_box : collision.bounding_boxes) {
                min_x = std::min(min_x, bounding_box.z);
                max_x = std::max(max_x, bounding_box.z + bounding_box.x);
                min_y = std::min(min_y, bounding_box.w);
                max_y = std::max(max_y, bounding_box.w + bounding_box.y);
            }

            lightgrid::bounds bounds{
                static_cast<int>(spacial.pos.x + min_x), 
                static_cast<int>(spacial.pos.y + min_y), 
                static_cast<int>(max_x - min_x), 
                static_cast<int>(max_y - min_y) 
            };
            return bounds;
        }
    );
};