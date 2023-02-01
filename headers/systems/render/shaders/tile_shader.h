#pragma once

#include <string>

#include "shader_program.h"

class TileShader : public ShaderProgram<glm::mat4, glm::mat4, glm::mat4, glm::vec2> {
public:
    TileShader();

    void renderSetup(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec2 texData);

private:

    GLint MVPuniformID;
    GLint MuniformID;
    GLint VuniformID;

    GLuint tileColorUniformID;
    GLuint texDataUniformID;
};