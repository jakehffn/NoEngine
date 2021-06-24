#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Animation {
    
    std::vector<int> frameOrder;

    // In seconds
    double deltaTime{0};
    float frameSpeed{1.0/8.0};

    // current animation frame
    int currAnimFrame{0};
};