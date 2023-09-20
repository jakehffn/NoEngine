#pragma once

#include <functional>
#include <concepts>

#include <entt/entt.hpp>

#include "state_machine.hpp"

class StateMachineBuilder {
public:
    StateMachineBuilder* then(std::function<void(entt::registry&, entt::entity)> action);
    template<std::invocable<entt::registry&, entt::entity>...Actions>
    StateMachineBuilder* choose(Actions...actions);
    StateMachineBuilder* wait(double ms);
    // StateMachineBuilder wait_uniform(double low_ms, double high_ms);
    
    StateMachine loop();
    StateMachine once();
    StateMachine destroy();

private:
    StateMachine state_machine;
};