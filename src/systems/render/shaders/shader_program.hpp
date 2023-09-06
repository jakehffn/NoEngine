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
        std::function<void(ShaderProgram*, size_t, GLuint, GLuint, GLuint)> setup_function
    );
    void use();
    void render(size_t num_verts, GLuint vao, GLuint dest_fbo, GLuint prev_texture);
    void recompile();
    void setUniformId(const char* name, GLuint id);
    GLuint getUniformId(const char* name);

private:

    GLuint id;
    std::unordered_map<std::string, GLuint> uniforms;
    std::function<GLuint(ShaderProgram*)> init_function;
    std::function<void(ShaderProgram*, size_t, GLuint, GLuint, GLuint)> setup_function;
};