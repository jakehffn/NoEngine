#pragma once

#include <glm/glm.hpp>

// Data about the frame's location in the texture atlas texture
struct AtlasData {
    glm::ivec2 position;
    glm::ivec2 size; // Width, height
    glm::ivec2 offset;
};