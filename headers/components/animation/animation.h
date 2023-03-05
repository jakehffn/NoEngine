#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "animation_structs.h"
#include "animator.h"

struct Animation {
    Animator* animator; // The animator which controls this animation
    AnimationData* animationData; // Data for the currently set animation
};