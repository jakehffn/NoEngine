#include "movement_system.hpp"

MovementSystem::MovementSystem(entt::registry& registry) : System(registry),
    velocity_observer{ entt::observer(registry, entt::collector.group<Velocity, Spacial>()) } {}

void MovementSystem::update() {
    DEBUG_TIMER(_, "MovementSystem::update");
    auto velocity_entities = this->registry.view<Velocity, Spacial>();

    for (auto entity : velocity_entities) {

        auto& velocity = velocity_entities.get<Velocity>(entity);
        float delta_time = this->registry.ctx().at<Clock&>().getDeltaTime();

        this->registry.patch<Spacial>(entity, [velocity, delta_time](auto &spacial) { 

            spacial.position += velocity.components * delta_time / 1000.0f;
        });
    }
}