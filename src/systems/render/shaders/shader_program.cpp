#include "shader_program.hpp"

ShaderProgram::ShaderProgram(
    std::function<GLuint(ShaderProgram*)> init_function, 
    std::function<void(ShaderProgram*, size_t, GLuint, GLuint, GLuint)> setup_function
) : init_function{init_function}, setup_function{setup_function} {
    this->id = this->init_function(this);
}

void ShaderProgram::use(){ 
    glUseProgram(this->id); 
};

void ShaderProgram::render(size_t num_verts, GLuint vao, GLuint dest_fbo, GLuint prev_texture){ 
    this->setup_function(this, num_verts, vao, dest_fbo, prev_texture); 
};

void ShaderProgram::recompile() {
    glDeleteShader(this->id);
    this->id = this->init_function(this);
}

void ShaderProgram::setUniform(const char* name, GLfloat* buffer_data) {
    this->uniforms[name] = buffer_data;
}

GLfloat* ShaderProgram::getUniform(const char* name) {
    return this->uniforms.at(name);
}

void ShaderProgram::setUniformId(const char* name, GLuint id) {
    this->uniform_locations[name] = id;
}

GLuint ShaderProgram::getUniformId(const char* name) {
    return this->uniform_locations.at(name);
}