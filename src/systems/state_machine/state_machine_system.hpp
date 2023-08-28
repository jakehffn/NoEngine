#pragma once

#include <entt\entt.hpp>

#include "system.hpp"
#include "state_machine.hpp"

class StateMachineSystem : public System {
public: 
    StateMachineSystem(entt::registry& registry);

    void update() override;
};