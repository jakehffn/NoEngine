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
    glm::vec3 position{0,0,0};
    // dimensions of entity
    glm::vec2 dimensions{1,1};
    // rotation
    glm::vec3 rotation{0,0,0};
    // scale
    glm::vec3 scale{1,1,1};
    // direction will usually be the same as last velocity
    DIRECTION direction = DOWN;
};