#pragma once

#include <vector>
#include <memory>

#include "states.hpp"

struct StateMachine {
    State* current_state;
    // Has to be shared for entt::registry to use
    std::vector<std::shared_ptr<State>> states;
};