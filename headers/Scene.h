#pragma once

#include <vector>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <entt\entt.hpp>

#include "Clock.h"
#include "Input.h"
#include "System.h"

class Scene {
public:
    Scene(SDL_Window* window);
    //, Clock* clock, Input* input, CameraController* cameraController

    void mainLoop();

private:
    Clock clock = Clock();
    Input input = Input();

    RenderSystem renderSystem;

    entt::registry registry;
};