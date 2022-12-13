#pragma once

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include "ShaderLoader.h"

// ShaderProgram provides a way for the Game to interface with each shader prorgam
//  This is done through the renderSetup() function
template <typename... Ts>
class ShaderProgram {
public:
    // Returns the ID for the shader program given by OpenGL once compiled
    // virtual GLuint getOpenGLShaderProgramID() = 0;

    inline void useShader(){ glUseProgram( this->openGLShaderProgramID ); };
    // Functions that provides ability to setup the render specifically for each shader program
    virtual void renderSetup(Ts... args) = 0;

protected:
    GLuint openGLShaderProgramID;
};