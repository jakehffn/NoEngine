#pragma once

struct Animator {
    static constexpr auto in_place_delete = true; // For pointer stability on deletion

    Animator(std::vector<float>* frame_durations) : 
        frame_durations{frame_durations}, num_frames{frame_durations->size()} {}

    std::vector<float>* frame_durations;
    size_t num_frames{1};
    // Total time frame has been visible
    double frame_time{0};
    // current animation frame
    size_t current_frame{0};
};