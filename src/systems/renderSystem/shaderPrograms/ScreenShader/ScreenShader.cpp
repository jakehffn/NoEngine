#include "ScreenShader.h"

ScreenShader::ScreenShader() {
    
    std::string vertexShaderPath = "./src/systems/renderSystem/shaderPrograms/ScreenShader/vertexShader.glsl";
    std::string fragmentShaderPath = "./src/systems/renderSystem/shaderPrograms/ScreenShader/fragmentShader.glsl";

    this->openGLShaderProgramID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
};