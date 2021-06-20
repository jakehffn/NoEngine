#pragma once

#include "SpriteObject.h"
#include "Input.h"
#include "Clock.h"

class PlayerObject : public SpriteObject {
public:
    PlayerObject(glm::vec3 position, Input* input, Clock* clock,
        GLuint shaderProgramID=0, glm::vec3 rotation=glm::vec3(0));
    void logic();
private:
    Input* input;
    Clock* clock;
};