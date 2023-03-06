#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <entt\entt.hpp>

#include "clock.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "texture_atlas.hpp"
#include "grid.hpp"
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

class Game {
public:
    Game(SDL_Window* window);

    void mainLoop();

private:
    SDL_Window* window;
    entt::registry registry;
    std::vector<System*> systems;

    Clock clock = Clock();
    Camera worldCamera = Camera();
    Camera guiCamera = Camera();
    Input inputManager = Input();
    // This should be read from the map at some point
    TextureAtlas textureAtlas = TextureAtlas("./src/assets/atlases/UntitledRPGAtlas/UntitledRPGAtlas");

    ComponentGrid<Renderable, Collision> componentGrid = ComponentGrid<Renderable, Collision>(this->registry);
};