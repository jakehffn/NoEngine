#include "screen_shader.hpp"

ScreenShader::ScreenShader() {
    
    std::string vertex_shader_path = "./src/systems/render/shaders/screen/vertexShader.glsl";
    std::string fragment_shader_path = "./src/systems/render/shaders/screen/fragmentShader.glsl";

    this->gl_shader_program_id = LoadShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());

    this->time_uniform_id = glGetUniformLocation(this->gl_shader_program_id, "time");
};

void ScreenShader::renderSetup(double time) {
    glUniform1f(this->time_uniform_id, time);
};