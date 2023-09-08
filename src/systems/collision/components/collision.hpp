#pragma once

#include <glm/glm.hpp>

struct Collision {
    // each glm::vec4 represents (width, height, XOffset, YOffset) for each collision box
    std::vector<glm::vec4> bounding_boxes; 
    // The grid queries can be cached in component so that collidable entities do not double query
    std::vector<entt::entity> current_grid_query;
};