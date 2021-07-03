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

#include "RenderSystem.h"
#include "InputSystem.h"
#include "StateSystem.h"
#include "CollisionSystem.h"

#include "create_entity.h"

class Scene {
public:
    Scene(SDL_Window* window);
    ~Scene();
    //, Clock* clock, Input* input, CameraController* cameraController

    void mainLoop();

    void loadTiledMap(const char* mapPath);

private:
    void addObject(tson::Object& obj);

    GLuint programID = 0;
    SDL_Window* window;

    Clock clock = Clock();

    RenderSystem* renderSystem;
    InputSystem* inputSystem;
    StateSystem* stateSystem;
    CollisionSystem* collisionSystem;

    entt::registry registry;
};