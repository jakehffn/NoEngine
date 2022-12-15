#pragma once 

#include <glm/glm.hpp>

enum DIRECTION {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Spacial {
    // position
    glm::vec3 pos;
    // rotation
    glm::vec3 rot;
    // scale
    glm::vec3 scale;
    // dimensions of entity
    glm::vec2 dim;
    // direction will usually be the same as last velocity
    DIRECTION direction = DOWN;
};