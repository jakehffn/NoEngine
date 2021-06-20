#pragma once

#include "Path.h"

class LinePath : public Path {
public:
    LinePath(glm::vec3 startPoint, glm::vec3 endPoint, float pathSpeed);
    void update(glm::vec3& pos, double time);

private:
    glm::vec3 startPoint;
    glm::vec3 endPoint;

    float pathSpeed;
};