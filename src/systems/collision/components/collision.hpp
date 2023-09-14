#pragma once

#include <glm/glm.hpp>

struct Collision {
    // each glm::vec4 represents (width, height, XOffset, YOffset) for each collision box
    std::vector<glm::vec4> bounding_boxes; 
    // REMINDER!!! Entities that have been collided with may no longer be valid by the end of the frame
    std::vector<entt::entity> collisions;
};