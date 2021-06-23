#pragma once

#include <vector>
#include <iostream>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <entt\entt.hpp>

#include "Clock.h"
#include "Input.h"

#include "RenderSystem.h"
#include "InputSystem.h"

#include "Sprite.h"
#include "Model.h"
#include "Spacial.h"
#include "CameraController.h"
#include "Animation.h"

class Scene {
public:
    Scene(SDL_Window* window);
    ~Scene();
    //, Clock* clock, Input* input, CameraController* cameraController

    void mainLoop();

private:
    GLuint programID = 0;
    SDL_Window* window;

    Clock clock = Clock();

    RenderSystem* renderSystem;
    InputSystem* inputSystem;

    entt::registry registry;
};