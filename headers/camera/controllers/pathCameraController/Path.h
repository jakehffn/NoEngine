#pragma once

#include <glm/glm.hpp>

class Path {
public:
    virtual void update(glm::vec3& pos, double time) = 0;
};