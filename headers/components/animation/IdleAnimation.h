#pragma once

#include <unordered_map>

#include "Spacial.h"
#include "Texture.h"

struct IdleAnimation {
    std::unordered_map<DIRECTION, Texture> animations;
};