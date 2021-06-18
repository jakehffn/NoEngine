#pragma once

#include "GameObject.h"
#include "Input.h"

class PlayerObject : public GameObject {
public:
    PlayerObject(glm::vec3 pos, Input* input);
    void logic();

private:
    Input* input;
};

