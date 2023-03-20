#pragma once 

#include <glm/glm.hpp>

enum DIRECTION {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Spacial {
    // position
    glm::vec3 pos{0,0,0};
    // rotation
    glm::vec3 rot{0,0,0};
    // scale
    glm::vec3 scale{1,1,1};
    // dimensions of entity
    glm::vec2 dim{1,1};
    // direction will usually be the same as last velocity
    DIRECTION direction = DOWN;
};