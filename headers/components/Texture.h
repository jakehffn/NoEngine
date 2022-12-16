#pragma once

#include <string>
#include <glm\glm.hpp>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>


struct Texture {

    std::string texturePath;

    int numFrames{1};

    int width;
    int height;
    int nColorChannels;

    GLuint glTextureID;

    glm::vec2 texData{0.0f, 1.0f};

    Texture(){}

    Texture(const Texture& texture) {

        this->width = texture.width;
        this->height = texture.height;
        this->nColorChannels = texture.nColorChannels;
        this->glTextureID = texture.glTextureID;

        this->texturePath = texture.texturePath;
        this->numFrames = texture.numFrames;
        this->texData = texture.texData;
    }

    Texture(std::string texturePath, int numFrames=1) {

        this->texturePath = texturePath;
        this->numFrames = numFrames;
        this->texData=glm::vec2(0.0f, 1.0f/numFrames);
    }

    // Copies data that must be retreived during init with stb
    void init(Texture texture) {
        
        this->width = texture.width;
        this->height = texture.height;
        this->nColorChannels = texture.nColorChannels;
        this->glTextureID = texture.glTextureID;
    }
};