#pragma once

#include <vector>

#include "states.hpp"

struct StateMachine {
    State* current_state;
    std::vector<State*> states;
};