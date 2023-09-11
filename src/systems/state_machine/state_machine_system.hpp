#pragma once

#include <entt\entt.hpp>

#include "system.hpp"
#include "state_machine.hpp"

#include "debug_timer.hpp"

class StateMachineSystem : public System {
public: 
    StateMachineSystem(entt::registry& registry);

    void update() override;
};