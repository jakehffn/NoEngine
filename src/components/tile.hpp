#pragma once

#include <glm/glm.hpp>

#include "texture.hpp"

struct Tile {
    int gid;
    glm::vec2 position; // The position of the tile in the texture
    Texture* tile_set_texture;
};