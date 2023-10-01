#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "spacial.hpp"
#include "atlas_data.hpp"

// Data about a specific animation sequence
struct AnimationData {
    std::string name;
    int num_frames;
    std::vector<float> frame_durations;
    std::vector<AtlasData*> frames;
};

using AnimationMap = std::unordered_map<std::string, AnimationData>;

struct SpriteSheet {
    std::string id;
    glm::ivec2 size; // Size of the original sprite sheet
    glm::ivec2 sprite_size; // Size of individual sprite
    AnimationMap animations;
};