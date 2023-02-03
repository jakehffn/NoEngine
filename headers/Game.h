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

#include "clock.h"
#include "camera.h"
#include "input.h"
#include "texture_atlas.h"
#include "grid.h"
#include "renderable.h"
#include "collision.h"

#include "input_system.h"
#include "render_system.h"
#include "collision_system.h"
#include "movement_system.h"
#include "animation_system.h"
#include "camera_system.h"
#include "gui_system.h"
#include "map_loader_system.h"

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