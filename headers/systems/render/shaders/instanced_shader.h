#pragma once

#include <string>

#include "shader_program.h"

class InstancedShader : public ShaderProgram<glm::mat4, glm::mat4, glm::vec2> {
public:
    InstancedShader();

    void renderSetup(glm::mat4 view, glm::mat4 projection, glm::vec2 atlasDimensions);

private:

    GLint VuniformID;
    GLint PuniformID;
    GLuint atlasDimensionsUniformID;
};