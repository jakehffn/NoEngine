#include "state_machine_builder.hpp"

StateMachineBuilder* StateMachineBuilder::then(std::function<void(entt::registry&, entt::entity)> action) {
    State* new_state = new ActionState(action);

    if (!this->state_machine.states.size() == 0) {
        this->state_machine.states.back()->next_state = new_state;
    }
    this->state_machine.states.push_back(new_state);

    return this;
}

template<std::invocable<entt::registry&, entt::entity>...Actions>
StateMachineBuilder* StateMachineBuilder::choose(Actions...actions) {
    State* new_state = new RandomState(
        std::vector<std::function<void(entt::registry&, entt::entity)>>{{actions...}}
    );

    if (!this->state_machine.states.size() == 0) {
        this->state_machine.states.back()->next_state = new_state;
    }
    this->state_machine.states.push_back(new_state);

    return this;
}

StateMachineBuilder* StateMachineBuilder::wait(double ms) {
    State* new_state = new WaitState(ms);

    if (!this->state_machine.states.size() == 0) {
        this->state_machine.states.back()->next_state = new_state;
    }
    this->state_machine.states.push_back(new_state);

    return this;
}

StateMachine StateMachineBuilder::loop() {
    this->state_machine.states.back()->next_state = this->state_machine.states.front();
    this->state_machine.current_state = this->state_machine.states.front();
    return this->state_machine;
}