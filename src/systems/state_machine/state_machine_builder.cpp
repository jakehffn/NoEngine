#include "state_machine_builder.hpp"

StateMachineBuilder& StateMachineBuilder::then(std::function<void(entt::registry&, entt::entity)> action) {
    auto new_state = std::make_shared<ActionState>(action);

    if (!this->state_machine.states.size() == 0) {
        this->state_machine.states.back()->next_state = new_state.get();
    }
    this->state_machine.states.push_back(std::move(new_state));

    return *this;
}

template<std::invocable<entt::registry&, entt::entity>...Actions>
StateMachineBuilder& StateMachineBuilder::choose(Actions...actions) {
    auto new_state = std::make_shared<RandomState>(
        std::vector<std::function<void(entt::registry&, entt::entity)>>{{actions...}}
    );

    if (!this->state_machine.states.size() == 0) {
        this->state_machine.states.back()->next_state = new_state.get();
    }
    this->state_machine.states.push_back(std::move(new_state));

    return *this;
}

StateMachineBuilder& StateMachineBuilder::wait(double ms) {
    auto new_state = std::make_shared<WaitState>(ms);

    if (!this->state_machine.states.size() == 0) {
        this->state_machine.states.back()->next_state = new_state.get();
    }
    this->state_machine.states.push_back(std::move(new_state));

    return *this;
}

StateMachine StateMachineBuilder::loop() {
    this->state_machine.states.back()->next_state = this->state_machine.states.front().get();
    this->state_machine.current_state = this->state_machine.states.front().get();
    return std::move(this->state_machine);
}

StateMachine StateMachineBuilder::once() {
    this->then([](entt::registry& registry, entt::entity entity) {
        registry.remove<StateMachine>(entity);
    });
    return std::move(this->state_machine);
}

StateMachine StateMachineBuilder::destroy() {
    this->then([](entt::registry& registry, entt::entity entity) {
        registry.destroy(entity);
    });
    this->state_machine.current_state = this->state_machine.states.front().get();
    return std::move(this->state_machine);
}