#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Animation {

    // In seconds
    float secondsPerFrame{1.0/10.0};
    // Total time frame has been visible
    double frameTime{0};
    // current animation frame
    int currentFrame{0};
};