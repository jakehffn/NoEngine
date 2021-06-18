#pragma once


#include "GameObject.h"

class PlayerObject : public GameObject {
public:
    PlayerObject(glm::vec3 pos);
    void logic();
};

