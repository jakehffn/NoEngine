#include "shader_program.hpp"

ShaderProgram::ShaderProgram(
    std::function<GLuint(ShaderProgram*)> init_function, 
    std::function<void(ShaderProgram*)> setup_function
) : init_function{init_function}, setup_function{setup_function} {
    this->id = this->init_function(this);
}

void ShaderProgram::use(){ 
    glUseProgram(this->id); 
};

void ShaderProgram::setup(){ 
    this->setup_function(this); 
};

void ShaderProgram::recompile() {
    glDeleteShader(this->id);
    this->id = this->init_function(this);
}

void ShaderProgram::setUniform(const char* name, GLuint id) {
    this->uniforms[name] = id;
}

GLuint ShaderProgram::getUniform(const char* name) {
    return this->uniforms.at(name);
}