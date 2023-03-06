#include "instanced_shader.hpp"

InstancedShader::InstancedShader() {
    
    std::string vertexShaderPath = "./src/systems/render/shaders/instanced/vertexShader.glsl";
    std::string fragmentShaderPath = "./src/systems/render/shaders/instanced/fragmentShader.glsl";

    this->openGLShaderProgramID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    this->VuniformID = glGetUniformLocation(this->openGLShaderProgramID, "V");
    this->PuniformID = glGetUniformLocation(this->openGLShaderProgramID, "P");

    this->atlasDimensionsUniformID = glGetUniformLocation(this->openGLShaderProgramID, "atlasDimensions");
}
void InstancedShader::renderSetup(glm::mat4 view, glm::mat4 projection, glm::vec2 atlasDimensions) {

    glUniformMatrix4fv(this->VuniformID, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(this->PuniformID, 1, GL_FALSE, &projection[0][0]);
    glUniform2f(this->atlasDimensionsUniformID, atlasDimensions.x, atlasDimensions.y);
}