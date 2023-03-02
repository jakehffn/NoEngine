#include "collision_system.h"

CollisionSystem::CollisionSystem(entt::registry& registry) : System(registry),
    collisionObserver{ entt::observer(registry, entt::collector.update<Spacial>().where<Collision>()) } {}

void CollisionSystem::update() {

    // Iterate over all entities were moved in the last frame and have Collision
    for (const auto observedEntity : this->collisionObserver) {

        
        auto entities = registry.view<Collision, Spacial>();

        auto [observedCollision, spacial] = entities.get<Collision, Spacial>(observedEntity);

        for (auto colEntity : entities) {

            if (colEntity != observedEntity) {

                auto [entityCollision, entitySpacial] = entities.get(colEntity);

                for (auto boundingBox : entityCollision.boundingBoxes) {

                    resolveCollision(observedCollision.boundingBoxes.at(0), spacial, boundingBox, entitySpacial);
                }
            }
        }  
    }

    this->collisionObserver.clear();
}

void CollisionSystem::resolveCollision(glm::vec4 collision1, Spacial& spacial1, glm::vec4 collision2, Spacial spacial2) {

    glm::vec3 pos1 = spacial1.pos;

    float t1 = pos1.y + collision1.w;
    float b1 = pos1.y + collision1.w + collision1.y;
    float l1 = pos1.x + collision1.z;
    float r1 = pos1.x + collision1.z + collision1.x;

    glm::vec3 pos2 = spacial2.pos;

    float t2 = pos2.y + collision2.w;
    float b2 = pos2.y + collision2.w + collision2.y;
    float l2 = pos2.x + collision2.z;
    float r2 = pos2.x + collision2.z + collision2.x;

    // float epsilon = 0.005;

    if (b1 > t2 && b2 > t1 && r1 > l2 && r2 > l1) {
        switch (spacial1.direction) {
            case UP:
                spacial1.pos.y = b2 - collision1.w;
                break;
            case DOWN:
                spacial1.pos.y = t2 - collision1.y - collision1.w;
                break;
            case LEFT:
                spacial1.pos.x = r2 - collision1.z;
                break;
            case RIGHT:
                spacial1.pos.x = l2 - collision1.x - collision1.z;
                break;
        }
    }
}