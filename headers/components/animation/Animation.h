#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "AnimationStructs.h"

struct Animation {

    // Total time frame has been visible
    double frameTime{0};
    // current animation frame
    int currentFrame{0};
    AnimationData animationData;
};