#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "animator.hpp"
#include "animation_structs.hpp"

struct Animation {
    Animator* animator; // The animator which controls this animation
    AnimationData* animation_data; // Data for the currently set animation
};