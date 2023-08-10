#pragma once

#include <unordered_map>

#include "animation.hpp"
#include "spacial.hpp"

#include "animation_structs.hpp"

struct MoveAnimation {
    std::unordered_map<DIRECTION, AnimationData> animations;
};