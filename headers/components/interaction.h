#pragma once

#include <vector>

#include <glm/glm.hpp>

struct Interaction {

    enum TYPE {
        GENERIC,
        ATTACK
    };

    // each glm::vec4 represents (width, height, XOffset, YOffset) for each collision box
    std::vector<glm::vec4> boundingBoxes; 
    TYPE type{GENERIC};

};