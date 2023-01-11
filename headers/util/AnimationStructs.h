#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Spacial.h"


struct FrameData {

    float duration;
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 offset;
};

struct AnimationData {

    std::string name;
    DIRECTION direction;
    int numFrames;
    std::vector<FrameData> frames;
};

struct SpriteSheet {

    glm::vec2 size;
    // glm::vec2 position; // Position of spritesheet in atlas
    glm::vec2 spriteSize; // Size of individual sprite

    // Animation name then direction
    std::unordered_map<std::string, std::unordered_map<DIRECTION, AnimationData>> animations;
};