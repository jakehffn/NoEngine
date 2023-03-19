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
    DIRECTION direction;
    int num_frames;
    std::vector<float> frame_durations;
    std::vector<AtlasData*> frames; // Unused for tilesets
};

using AnimationMap = std::unordered_map<std::string, std::unordered_map<DIRECTION, AnimationData>>;

struct SpriteSheet {

    std::string name;
    glm::ivec2 size;
    glm::ivec2 sprite_size; // Size of individual sprite

    // Animation name, then direction
    AnimationMap animations;
};