#pragma once

#include <string>
#include <glm\glm.hpp>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "AnimationStructs.h"

struct Texture {

    std::string spriteSheetName;

    int width;
    int height;

    FrameData frameData;
};