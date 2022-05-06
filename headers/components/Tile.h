#pragma once

#include <glm\glm.hpp>

struct Tile {

    glm::vec2 pos{0.0f, 0.0f}; // Position of tile in world
    int id{0}; // Tile id
};