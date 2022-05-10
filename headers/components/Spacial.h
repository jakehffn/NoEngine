#pragma once 

#include <glm/glm.hpp>

struct Spacial {
    // position
    glm::vec3 pos;
    // rotation
    glm::vec3 rot;
    // scale
    glm::vec3 scale;
    // dimensions of entity
    glm::vec2 dim;
};