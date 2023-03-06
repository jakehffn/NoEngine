#pragma once

struct Animator {
    static constexpr auto in_place_delete = true; // For pointer stability on deletion
    
    std::vector<float>* frame_durations;
    int num_frames{1};
    // Total time frame has been visible
    double frameTime{0};
    // current animation frame
    int currentFrame{0};
};