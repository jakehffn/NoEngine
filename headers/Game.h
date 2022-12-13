#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "tileson.hpp"

#include <entt\entt.hpp>

#include "Clock.h"
#include "InputManager.h"

#include "InputSystem.h"
#include "RenderSystem.h"
#include "CollisionSystem.h"
#include "MovementSystem.h"

#include "entities.h"

class Game {
public:
    Game(SDL_Window* window);
    ~Game();
    //, Clock* clock, Input* input, CameraController* cameraController

    void mainLoop();

    void loadTiledMap(const char* mapPath);

private:
    void addObjects(tson::Layer* objectLayer);
    void addTiles(tson::Layer* tileLayer);

    GLuint programID = 0;
    SDL_Window* window;

    Clock clock = Clock();
    InputManager inputManager = InputManager();

    std::vector<System*> systems;

    entt::registry registry;
};