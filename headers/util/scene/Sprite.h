#pragma once

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

// Used to hold specific model vertex data
class Sprite {
public:
    Sprite(char* spritePath);

    GLuint getVAO();
    GLuint getTexture();

private:
    int width;
    int height;
    int nColorChannels;

    GLuint VAO;
    GLuint texture;
};