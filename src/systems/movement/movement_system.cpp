#include "movement_system.hpp"

MovementSystem::MovementSystem(entt::registry& registry) : System(registry),
    velocityObserver{ entt::observer(registry, entt::collector.group<Velocity, Spacial>()) } {}

void MovementSystem::update() {

    auto velocityEntities = this->registry.view<Velocity, Spacial>();

    for (auto entity : velocityEntities) {

        auto& velocity = velocityEntities.get<Velocity>(entity);
        float deltaTime = this->registry.ctx().at<Clock&>().getDeltaTime();

        this->registry.patch<Spacial>(entity, [velocity, deltaTime](auto &spacial) { 

            spacial.pos += velocity.direction * velocity.magnitude * deltaTime;
        });
    }
}