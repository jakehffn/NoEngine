#include "TileShader.h"

TileShader::TileShader() {
    
    std::string vertexShaderPath = "./src/systems/renderSystem/shaderPrograms/TileShader/vertexShader.glsl";
    std::string fragmentShaderPath = "./src/systems/renderSystem/shaderPrograms/TileShader/fragmentShader.glsl";

    this->openGLShaderProgramID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    this->MVPuniformID = glGetUniformLocation(this->openGLShaderProgramID, "MVP");
    this->MuniformID = glGetUniformLocation(this->openGLShaderProgramID, "M");
    this->VuniformID = glGetUniformLocation(this->openGLShaderProgramID, "V");

    this->texDataUniformID = glGetUniformLocation(this->openGLShaderProgramID, "texData");
    this->tileColorUniformID = glGetUniformLocation(this->openGLShaderProgramID, "spriteColor");
}

void TileShader::renderSetup(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec2 texData) {
    
    glm::mat4 MVP = projection*view*model;

    glUniformMatrix4fv(this->MVPuniformID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(this->MuniformID, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(this->VuniformID, 1, GL_FALSE, &view[0][0]);

    glUniform3f(this->tileColorUniformID, 1, 1, 1);
    glUniform2f(this->texDataUniformID, texData.x, texData.y);
}