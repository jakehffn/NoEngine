#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Animation {
    
    std::vector<int> frameOrder;

    // In seconds
    float frameSpeed{1.0/10.0};
    double deltaTime{0};

    // current animation frame
    int currAnimFrame{0};
};