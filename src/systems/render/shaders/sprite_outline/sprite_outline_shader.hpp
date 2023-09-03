#pragma once

#include <string>

#include "shader_program.hpp"

class SpriteOutlineShader : public ShaderProgram<glm::mat4, glm::mat4, glm::vec2> {
public:
    SpriteOutlineShader();

    void renderSetup(glm::mat4 view, glm::mat4 projection, glm::vec2 atlas_dimensions);

private:
    GLint v_uniform_id;
    GLint p_uniform_id;
    GLuint atlas_dimensions_uniform_id;
};