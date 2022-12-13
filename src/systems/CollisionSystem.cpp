#include "CollisionSystem.h"

CollisionSystem::CollisionSystem(entt::registry& registry) : System(registry),
    collisionObserver{ entt::observer(registry, entt::collector.update<Spacial>().where<Collision>()) } {}

void CollisionSystem::update() {

    // Iterate over all entities were moved in the last frame and have Collision
    for (const auto observedEntity : this->collisionObserver) {

        auto [observedCollision, spacial] = this->registry.get<Collision, Spacial>(observedEntity);

        auto entities = registry.view<Collision, Spacial>();

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

void CollisionSystem::resolveCollision(glm::vec4 collision, Spacial& spacial, glm::vec4 entityCol, Spacial entitySpac) {

    glm::vec3 offset1(collision.z, collision.w, 0);
    glm::vec3 offset2(entityCol.z, entityCol.w, 0);

    // Collision is based on top left corner while rendered obejcts are based on bottom left
    //  This might need to be a temporary solution, as this will not work for two rendered objects colliding
    glm::vec3 collisionOffset(0, spacial.dim.y, 0);

    glm::vec3 pos1 = spacial.pos + offset1;
    glm::vec2 dim1{collision};

    glm::vec3 pos2 = entitySpac.pos + offset2;
    glm::vec2 dim2{entityCol};

    if (pos1.y < pos2.y + dim2.y && pos1.y + dim1.y > pos2.y && pos1.x < pos2.x + dim2.x && pos1.x + dim1.x > pos2.x) {
        switch (spacial.direction) {
            case UP:
                spacial.pos.y = (pos2.y + dim2.y) - offset1.y;
                break;
            case DOWN:
                spacial.pos.y = (pos2.y - dim1.y) - offset1.y;
                break;
            case LEFT:
                spacial.pos.x = (pos2.x + dim2.x) - offset1.x;
                break;
            case RIGHT:
                spacial.pos.x = (pos2.x - dim1.x) - offset1.x;
                break;
        }
    }
}