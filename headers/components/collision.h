#pragma once

#include <glm/glm.hpp>

struct Collision {

    // each glm::vec4 represents (width, height, XOffset, YOffset) for each collision box
    std::vector<glm::vec4> boundingBoxes; 
};