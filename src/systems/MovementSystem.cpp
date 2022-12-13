#include "MovementSystem.h"

MovementSystem::MovementSystem(entt::registry& registry) : System(registry),
    velocityObserver{ entt::observer(registry, entt::collector.group<Velocity, Spacial>()) } {}

void MovementSystem::update() {

    this->velocityObserver.each([this](const auto entity) {
        
        float deltaTime = this->registry.ctx().at<Clock&>().getDeltaTime();
        const auto& velocity = registry.get<Velocity>(entity);

        DIRECTION dir;

        if (glm::abs(velocity.direction.y) > glm::abs(velocity.direction.x)) {
            dir = (velocity.direction.y < 0) ? UP : DOWN;
        } else {
            dir = (velocity.direction.x < 0) ? LEFT : RIGHT;
        }

        this->registry.patch<Spacial>(entity, [velocity, dir, deltaTime](auto &spacial) { 

            spacial.pos += velocity.direction * velocity.magnitude * deltaTime;
            spacial.direction = dir;
        });

        this->registry.remove<Velocity>(entity);
    });

}