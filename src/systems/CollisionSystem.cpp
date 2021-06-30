#include "CollisionSystem.h"

CollisionSystem::CollisionSystem(entt::registry& registry) :
    collisionObserver{ entt::observer(registry, entt::collector.update<Spacial>().where<Collision>()) } {}

void CollisionSystem::update(entt::registry& registry) {

    for (const auto entity : collisionObserver) {

        auto [collision, spacial] = registry.get<Collision, Spacial>(entity);

        resolveCollision(collision, spacial);
    }

    collisionObserver.clear();
}

void CollisionSystem::resolveCollision(Collision collision, Spacial& spacial) {
    printf("I have moved\n");
}

void CollisionSystem::systemState() {}