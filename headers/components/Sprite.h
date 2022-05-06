#pragma once

#include <glm\glm.hpp>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>


struct Sprite {

    glm::vec2 texData{0.0f, 1.0f};
    int numSprites;

    int width;
    int height;
    int nColorChannels;

    GLuint texture;
};