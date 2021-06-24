#pragma once

#include <glm\glm.hpp>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "consts.h"

class Sprite {
public:
    Sprite(const char* spritePath, int numSprites=1);

    GLuint getVAO() const;
    GLuint getTexture() const;

    int getWidth() const;
    int getHeight() const;

    glm::vec2 texData{0.0f, 1.0f};
    int numSprites;

private:
    int width;
    int height;
    int nColorChannels;

    GLuint VAO;
    GLuint texture;
};