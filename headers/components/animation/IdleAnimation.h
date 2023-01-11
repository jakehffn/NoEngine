#pragma once

#include <unordered_map>
#include "Animation.h"
#include "Spacial.h"

#include "AnimationStructs.h"

struct IdleAnimation {
    std::unordered_map<DIRECTION, AnimationData> animations;
};