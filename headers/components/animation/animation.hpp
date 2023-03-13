#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "animation_structs.hpp"
#include "animator.hpp"

struct Animation {
    Animator* animator; // The animator which controls this animation
    AnimationData* animation_data; // Data for the currently set animation
};