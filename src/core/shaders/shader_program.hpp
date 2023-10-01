#pragma once

#include <vector>
#include <functional>
#include <string>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>

#include "uniform.hpp"
#include "shader_loader.hpp"

class ShaderProgram {
public:
    ShaderProgram(
        const char* vertex_source, 
        const char* fragment_source,
        std::vector<std::string>& logs
    );
    ~ShaderProgram();
    void use();
    void render(size_t num_verts, GLuint vao, GLuint dest_fbo);
    void recompile();
    bool containsUniform(const char* name);

    template <typename T>
    void setUniform(const char* name, T uniform_data);
    template <typename T>
    void setUniform(const char* name, T* uniform_data);

private:
    void setUniformBufferData(size_t offset, GLint uniform_data);
    void setUniformBufferDataV(size_t offset, size_t count, GLint* uniform_data);
    void setUniformBufferData(size_t offset, GLfloat uniform_data);
    void setUniformBufferDataV(size_t offset, size_t count, GLfloat* uniform_data);
    void setUniformBufferData(size_t offset, glm::vec2 uniform_data);
    void setUniformBufferDataV(size_t offset, size_t count, glm::vec2* uniform_data);
    void setUniformBufferData(size_t offset, glm::vec3 uniform_data);
    void setUniformBufferDataV(size_t offset, size_t count, glm::vec3* uniform_data);
    void setUniformBufferData(size_t offset, glm::vec4 uniform_data);
    void setUniformBufferDataV(size_t offset, size_t count, glm::vec4* uniform_data);
    void setUniformBufferData(size_t offset, glm::mat4 uniform_data);
    void setUniformBufferDataV(size_t offset, size_t count, glm::mat4* uniform_data);
    void setUniformBufferData(size_t offset, GLuint uniform_data);

    void initUniformBuffer();
    void getUniforms();

    GLuint id;
    std::vector<Uniform> uniforms;

    size_t buffer_size;
    char* uniform_buffer;

    std::vector<std::string>& logs;

    const char* vertex_source;
    const char* fragment_source;
};

template<typename T>
void ShaderProgram::setUniform(const char* name, T uniform_data) {
    for (auto uniform : this->uniforms) {
        if (!strcmp(uniform.name.c_str(), name)) {
            this->setUniformBufferData(uniform.buffer_offset, uniform_data);
        }
    }
}

template<typename T>
void ShaderProgram::setUniform(const char* name, T* uniform_data) {
    for (auto uniform : this->uniforms) {
        if (!strcmp(uniform.name.c_str(), name)) {
            this->setUniformBufferDataV(
                uniform.buffer_offset, 
                uniform.array_size, 
                uniform_data
            );
        }
    }
}