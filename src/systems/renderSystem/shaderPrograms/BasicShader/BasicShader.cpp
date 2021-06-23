#include "BasicShader.h"

BasicShader::BasicShader() {
    
    std::string vertexShaderPath = "./src/systems/renderSystem/shaderPrograms/BasicShader/vertexShader.glsl";
    std::string fragmentShaderPath = "./src/systems/renderSystem/shaderPrograms/BasicShader/fragmentShader.glsl";

    this->openGLShaderProgramID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    this->MVPuniformID = glGetUniformLocation(this->openGLShaderProgramID, "MVP");
    this->MuniformID = glGetUniformLocation(this->openGLShaderProgramID, "M");
    this->VuniformID = glGetUniformLocation(this->openGLShaderProgramID, "V");

    this->frameDataUniformID = glGetUniformLocation(this->openGLShaderProgramID, "frameData");
    this->spriteColorUniformID = glGetUniformLocation(this->openGLShaderProgramID, "spriteColor");
}

GLuint BasicShader::getOpenGLShaderProgramID() {
    return this->openGLShaderProgramID;
}

void BasicShader::renderSetup(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec2 frameData) {
    
    glm::mat4 MVP = projection*view*model;

    glUniformMatrix4fv(this->MVPuniformID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(this->MuniformID, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(this->VuniformID, 1, GL_FALSE, &view[0][0]);

    glUniform3f(this->spriteColorUniformID, 1, 1, 1);
    glUniform2f(this->frameDataUniformID, frameData.x, frameData.y);
}