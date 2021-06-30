#pragma once

#include <glm/glm.hpp>

struct Collision {
    // width, height
    glm::vec2 dim;
    // offset x, y
    glm::vec2 offset{0, 0};
};