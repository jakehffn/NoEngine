#pragma once

#include <functional>
#include <string>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>

#include "shader_loader.hpp"

class ShaderProgram {
public:
    ShaderProgram(
        std::function<GLuint(ShaderProgram*)> init_function, 
        std::function<void(ShaderProgram*)> setup_function
    );
    void use();
    void setup();
    void recompile();
    void setUniform(const char* name, GLuint id);
    GLuint getUniform(const char* name);

private:
    GLuint id;
    std::unordered_map<std::string, GLuint> uniforms;
    std::function<GLuint(ShaderProgram*)> init_function;
    std::function<void(ShaderProgram*)> setup_function;
};