#include "CollisionSystem.h"

CollisionSystem::CollisionSystem(entt::registry& registry) :
    collisionObserver{ entt::observer(registry, entt::collector.update<Spacial>().where<Collision, SpriteState>()) } {}

void CollisionSystem::update(entt::registry& registry) {

    for (const auto entity : collisionObserver) {

        auto [collision, spacial, spriteState] = registry.get<Collision, Spacial, SpriteState>(entity);

        auto entities = registry.view<Collision, Spacial>();

        for (auto colEntity : entities) {

            if (colEntity != entity) {

                auto [entityCollision, entitySpacial] = entities.get(colEntity);

                resolveCollision(spriteState.state, collision, spacial, entityCollision, entitySpacial);
            }
        }  
    }

    collisionObserver.clear();
}

void CollisionSystem::resolveCollision(SpriteStatePair state, Collision collision, Spacial& spacial, Collision entityCol, Spacial entitySpac) {

    entity_c::ENTITY_DIR dir = std::get<entity_c::ENTITY_DIR>(state);

    glm::vec3 offset(collision.offset.x, collision.offset.y, 0);

    glm::vec3 pos1 = spacial.pos + offset;
    glm::vec2 dim1 = collision.dim;

    glm::vec3 pos2 = entitySpac.pos;
    glm::vec2 dim2 = entityCol.dim;

    if (pos1.y < pos2.y + dim2.y && pos1.y + dim1.y > pos2.y && pos1.x < pos2.x + dim2.x && pos1.x + dim1.x > pos2.x) {
        switch (dir) {
            case entity_c::UP:
                spacial.pos.y = (pos2.y + dim2.y) - offset.y;
                break;
            case entity_c::DOWN:
                spacial.pos.y = (pos2.y - dim1.y) - offset.y;
                break;
            case entity_c::LEFT:
                spacial.pos.x = (pos2.x + dim2.x) - offset.x;
                break;
            case entity_c::RIGHT:
                spacial.pos.x = (pos2.x - dim1.x) - offset.x;
                break;
        }
    }
}

void CollisionSystem::systemState() {}