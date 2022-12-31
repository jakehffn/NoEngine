#pragma once

#include <string>
#include <vector>

#include <glm\glm.hpp>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

struct TileSet {
    std::vector<glm::vec3> tileData;
    GLuint tileVBO = -1;
};