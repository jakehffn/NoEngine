#include "ScreenShader.h"

ScreenShader::ScreenShader() {
    
    std::string vertexShaderPath = "./src/systems/renderSystem/shaderPrograms/ScreenShader/vertexShader.glsl";
    std::string fragmentShaderPath = "./src/systems/renderSystem/shaderPrograms/ScreenShader/fragmentShader.glsl";

    this->openGLShaderProgramID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    this->MVPuniformID = glGetUniformLocation(this->openGLShaderProgramID, "MVP");
    this->MuniformID = glGetUniformLocation(this->openGLShaderProgramID, "M");
    this->VuniformID = glGetUniformLocation(this->openGLShaderProgramID, "V");

    this->texDataUniformID = glGetUniformLocation(this->openGLShaderProgramID, "texData");
    this->spriteColorUniformID = glGetUniformLocation(this->openGLShaderProgramID, "spriteColor");
}

GLuint ScreenShader::getOpenGLShaderProgramID() {
    return this->openGLShaderProgramID;
}

void ScreenShader::renderSetup(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec2 texData) {
    
    glm::mat4 MVP1 = projection*view*model;
    glm::mat4 MVP{1,0,0,0,
                  0,1,0,0,
                  0,0,1,0,
                  0,0,0,1};

    std::cout << glm::to_string(MVP1) << std::endl;
    std::cout << glm::to_string(MVP) << std::endl;
    // glm::mat4 MVP = model;

    // glm::vec4 test = MVP * glm::vec4(0, 1, 0, 1);

    glUniformMatrix4fv(this->MVPuniformID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(this->MuniformID, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(this->VuniformID, 1, GL_FALSE, &view[0][0]);

    glUniform3f(this->spriteColorUniformID, 1, 1, 1);
    glUniform2f(this->texDataUniformID, texData.x, texData.y);
}