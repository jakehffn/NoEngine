#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "animation_structs.h"

struct Animation {

    // Total time frame has been visible
    double frameTime{0};
    // current animation frame
    int currentFrame{0};
    AnimationData animationData;
};