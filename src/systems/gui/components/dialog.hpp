#pragma once

#include <vector>
#include <memory>

#include "dialog_step.hpp"

struct Dialog {
    static constexpr auto in_place_delete = true; // For pointer stability on deletion

    DialogStep* current_step{NULL};
    std::vector<std::shared_ptr<DialogStep>> steps;
    std::vector<entt::entity> children;

    size_t current_line_number{0};

    float width;
    bool prevent_interaction;
};