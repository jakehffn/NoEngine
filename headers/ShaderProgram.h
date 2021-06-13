#pragma once

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include "ShaderLoader.h"

// ShaderProgram provides a way for the Scene to interface with each shader prorgam
//  This is done through the renderSetup() function
class ShaderProgram {
public:
    // Returns the ID for the shader program given by OpenGL once compiled
    virtual GLuint getOpenGLShaderProgramID() = 0;
    // Functions that provides ability to setup the render specifically for each shader program
    virtual void renderSetup(glm::mat4 model, glm::mat4 view, glm::mat4 projection) = 0;
};