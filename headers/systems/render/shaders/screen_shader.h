#pragma once

#include <string>
#include <iostream>

#include "glm/gtx/string_cast.hpp"

#include "shader_program.h"

class ScreenShader : public ShaderProgram<double> {
public:
    ScreenShader();

    void renderSetup(double time);

private:
    GLint timeUniformID;
};