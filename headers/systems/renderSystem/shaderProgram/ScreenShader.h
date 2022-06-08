#pragma once

#include "ShaderProgram.h"
#include "glm/gtx/string_cast.hpp"
#include <string>
#include <iostream>

class ScreenShader : public ShaderProgram {
public:
    ScreenShader();

    GLuint getOpenGLShaderProgramID();
    void renderSetup(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec2 texData);

private:
    GLuint openGLShaderProgramID;

    GLint MVPuniformID;
    GLint MuniformID;
    GLint VuniformID;

    GLuint spriteColorUniformID;
    GLuint texDataUniformID;
};