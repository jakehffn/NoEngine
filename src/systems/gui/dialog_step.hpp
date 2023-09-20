#pragma once

#include <memory>
#include <functional>

#include <entt/entt.hpp>

struct DialogStep {
    virtual ~DialogStep() = 0;
    DialogStep* next_step{NULL};
    virtual DialogStep* next() = 0;
};

struct DialogText : public DialogStep {
    double text_speed{50.0};
    // list of text entities which allows for line breaks
    std::vector<entt::entity> text_entities;
};