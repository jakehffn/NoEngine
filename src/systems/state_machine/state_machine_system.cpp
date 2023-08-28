#include "state_machine_system.hpp"

StateMachineSystem::StateMachineSystem(entt::registry& registry) : System(registry) {}

void StateMachineSystem::update() {
    this->registry.view<StateMachine>().each([this](auto entity, auto& state_machine) {
        state_machine.current_state = state_machine.current_state->next(this->registry, entity);
    });
}