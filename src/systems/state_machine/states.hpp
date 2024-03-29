#pragma once

#include <functional>
#include <memory>

#include <entt/entt.hpp>

#include "clock.hpp"

struct State {
    virtual ~State() {};
    State* next_state{NULL};
    virtual State* next(entt::registry& registry, entt::entity entity) = 0;
};

struct WaitState : public State {
    double timer;
    double timer_reset;

    WaitState(double ms) : timer{ms}, timer_reset{ms} {}

    State* next(entt::registry& registry, entt::entity entity) {
        Clock& clock = registry.ctx().at<Clock&>();
        this->timer -= clock.getDeltaTime();
        if (this->timer <= 0) {
            this->timer = this->timer_reset;
            return this->next_state;
        } else {
            return this;
        }
    }
};

struct ActionState : public State {    
    ~ActionState() = default;
    std::function<void(entt::registry&, entt::entity)> action;

    ActionState(std::function<void(entt::registry&, entt::entity)> action) : 
        action{action} {}

    State* next(entt::registry& registry, entt::entity entity) {
        this->action(registry, entity);
        return this->next_state;
    }
};

struct RandomState : public State {
    ~RandomState() = default;
    std::vector<std::function<void(entt::registry&, entt::entity)>> actions;

    RandomState(std::vector<std::function<void(entt::registry&, entt::entity)>> actions) :
        actions{actions} {}

    
    State* next(entt::registry& registry, entt::entity entity) {
        this->actions[0](registry, entity);
        return this->next_state;
    }
};

struct FinalState : public State {
    ~FinalState() = default;
    State* next(entt::registry& registry, entt::entity entity) {
        return this;
    }
};