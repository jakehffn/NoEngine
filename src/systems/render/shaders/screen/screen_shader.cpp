#include "screen_shader.hpp"

ScreenShader::ScreenShader() {
    
    std::string vertexShaderPath = "./src/systems/render/shaders/screen/vertexShader.glsl";
    std::string fragmentShaderPath = "./src/systems/render/shaders/screen/fragmentShader.glsl";

    this->openGLShaderProgramID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    this->timeUniformID = glGetUniformLocation(this->openGLShaderProgramID, "time");
};

void ScreenShader::renderSetup(double time) {
    glUniform1f(this->timeUniformID, time);
};