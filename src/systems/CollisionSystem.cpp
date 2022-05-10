#include "CollisionSystem.h"

CollisionSystem::CollisionSystem(entt::registry& registry) :
    collisionObserver{ entt::observer(registry, entt::collector.update<Spacial>().where<Collision, SpriteState>()) } {}

void CollisionSystem::update(entt::registry& registry) {

    for (const auto entity : this->collisionObserver) {

        auto [collision, spacial, spriteState] = registry.get<Collision, Spacial, SpriteState>(entity);

        auto entities = registry.view<Collision, Spacial>();

        for (auto colEntity : entities) {

            // printf("col entity\n");

            if (colEntity != entity) {

                auto [entityCollision, entitySpacial] = entities.get(colEntity);

                resolveCollision(spriteState.state, collision, spacial, entityCollision, entitySpacial);
            }
        }  
    }

    this->collisionObserver.clear();
}

void CollisionSystem::resolveCollision(SpriteStatePair state, Collision collision, Spacial& spacial, Collision entityCol, Spacial entitySpac) {

    entity_c::ENTITY_DIR dir = std::get<entity_c::ENTITY_DIR>(state);

    glm::vec3 offset1(collision.offset.x, collision.offset.y, 0);
    glm::vec3 offset2(entityCol.offset.x, entityCol.offset.y, 0);

    // Collision is based on top left corner while rendered obejcts are based on bottom left
    //  This might need to be a temporary solution, as this will not work for two rendered objects colliding
    glm::vec3 collisionOffset(0, spacial.dim.y, 0);

    glm::vec3 pos1 = spacial.pos + offset1;
    glm::vec2 dim1 = collision.dim;

    glm::vec3 pos2 = entitySpac.pos + offset2;
    glm::vec2 dim2 = entityCol.dim;

    if (pos1.y < pos2.y + dim2.y && pos1.y + dim1.y > pos2.y && pos1.x < pos2.x + dim2.x && pos1.x + dim1.x > pos2.x) {
        switch (dir) {
            case entity_c::UP:
                spacial.pos.y = (pos2.y + dim2.y) - offset1.y;
                break;
            case entity_c::DOWN:
                spacial.pos.y = (pos2.y - dim1.y) - offset1.y;
                break;
            case entity_c::LEFT:
                spacial.pos.x = (pos2.x + dim2.x) - offset1.x;
                break;
            case entity_c::RIGHT:
                spacial.pos.x = (pos2.x - dim1.x) - offset1.x;
                break;
        }
    }
}

void CollisionSystem::systemState() {}