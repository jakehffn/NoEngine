#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "spacial.hpp"

struct FrameData {

    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 offset;
};

struct AnimationData {

    std::string name;
    DIRECTION direction;
    int num_frames;
    std::vector<float> frame_durations;
    std::vector<FrameData> frames;
};

using AnimationMap = std::unordered_map<std::string, std::unordered_map<DIRECTION, AnimationData>>;

struct SpriteSheet {

    std::string name;
    glm::vec2 size;
    glm::vec2 sprite_size; // Size of individual sprite

    // Animation name, then direction
    AnimationMap animations;
};