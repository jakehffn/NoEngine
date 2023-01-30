#pragma once

#include "ShaderProgram.h"
#include "glm/gtx/string_cast.hpp"
#include <string>
#include <iostream>

class ScreenShader : public ShaderProgram<double> {
public:
    ScreenShader();

    void renderSetup(double time);

private:
    GLint timeUniformID;
};