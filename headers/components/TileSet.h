#pragma once

#include <vector>
#include <glm/glm.hpp>

struct TileSet {

    int width;
    int height;
    std::vector<glm::vec3> tiles;
    int firstGID;
};