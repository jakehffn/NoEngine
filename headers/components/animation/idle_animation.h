#pragma once

#include <unordered_map>

#include "animation.h"
#include "spacial.h"

#include "animation_structs.h"

struct IdleAnimation {
    std::unordered_map<DIRECTION, AnimationData> animations;
};