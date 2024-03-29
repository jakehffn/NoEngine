#include "state_machine_system.hpp"

StateMachineSystem::StateMachineSystem(entt::registry& registry) : System(registry) {}

void StateMachineSystem::update() {
    DEBUG_TIMER(_, "StateMachineSystem::update");
    this->registry.view<StateMachine>().each([this](auto entity, auto& state_machine) {
        auto next_state = state_machine.current_state->next(this->registry, entity);
        if (next_state != NULL) {
            state_machine.current_state = next_state;
        }
    });
}