#pragma once

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Scene.h"

#include "Camera.h"
#include "UserCameraController.h"
#include "Clock.h"
#include "BasicShader.h"

#include "UserCameraController.h"
#include "TestMap.h"

class Game {
public:
    ~Game();
    void init(SDL_Window* window);
    void run();

private:
    GLuint programID = 0;
    SDL_Window* window = NULL;

    Clock* clock;
    Input* input;
    Scene* scene;

    CameraController* cameraController;
};