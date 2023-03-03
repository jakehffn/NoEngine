#include "collision_system.h"

CollisionSystem::CollisionSystem(entt::registry& registry) : System(registry),
    collision_observer{ entt::observer(registry, entt::collector.update<Spacial>().where<Collision>()) } {}

void CollisionSystem::update() {

    auto entities = registry.view<Collision, Spacial>();
    auto& component_grid = this->registry.ctx().at<ComponentGrid<Renderable, Collision>&>();

    // Iterate over all entities were moved in the last frame and have Collision
    for (const auto observed_entity : this->collision_observer) {

        auto [observed_collision, observed_spacial] = entities.get<Collision, Spacial>(observed_entity);

        // Iterate over the bounding boxes in the observed entity
        for (const auto& observed_bounding_box : observed_collision.boundingBoxes) {

            this->collision_query.clear();

            component_grid.query<Collision>((struct Bounds) {
                observed_spacial.pos.x + observed_bounding_box.z,
                observed_spacial.pos.y + observed_bounding_box.w,
                observed_bounding_box.x, observed_bounding_box.y}, this->collision_query);
            
            // Iterate over the entities which could be colliding with the observed entity
            for (auto other_entity : this->collision_query) {

                if (other_entity == observed_entity) {
                    continue;
                }

                auto [other_collision, other_spacial] = entities.get<Collision, Spacial>(other_entity);

                for (auto other_bounding_box : other_collision.boundingBoxes) {

                    if (this->isColliding(observed_bounding_box, observed_spacial, other_bounding_box, other_spacial)) {

                        this->resolveCollision(observed_bounding_box, observed_spacial, other_bounding_box, other_spacial);
                    }
                }
            }
        }
        

        // for (auto colEntity : entities) {

        //     if (colEntity != observed_entity) {

        //         auto [entity_collision, entity_spacial] = entities.get(colEntity);

        //         for (auto bounding_box : entity_collision.boundingBoxes) {

        //             resolveCollision(observed_collision.boundingBoxes.at(0), observed_spacial, bounding_box, entity_spacial);
        //         }
        //     }
        // }  
    }

    this->collision_observer.clear();
}

void CollisionSystem::resolveCollision(const glm::vec4& collision_1, Spacial& spacial_1, const glm::vec4& collision_2, const Spacial& spacial_2) {

    switch (spacial_1.direction) {
        case UP:
            spacial_1.pos.y = spacial_2.pos.y + collision_2.w + collision_2.y - collision_1.w;
            break;
        case DOWN:
            spacial_1.pos.y = spacial_2.pos.y + collision_2.w - collision_1.y - collision_1.w;
            break;
        case LEFT:
            spacial_1.pos.x = spacial_2.pos.x + collision_2.z + collision_2.x - collision_1.z;
            break;
        case RIGHT:
            spacial_1.pos.x = spacial_2.pos.x + collision_2.z - collision_1.x - collision_1.z;
            break;
    }
}

bool CollisionSystem::isColliding(const glm::vec4& collision_1, const Spacial& spacial_1, const glm::vec4& collision_2, const Spacial& spacial_2) {

    float top_1 = spacial_1.pos.y + collision_1.w;
    float bottom_1 = spacial_1.pos.y + collision_1.w + collision_1.y;
    float left_1 = spacial_1.pos.x + collision_1.z;
    float right_1 = spacial_1.pos.x + collision_1.z + collision_1.x;

    float top_2 = spacial_2.pos.y + collision_2.w;
    float bottom_2 = spacial_2.pos.y + collision_2.w + collision_2.y;
    float left_2 = spacial_2.pos.x + collision_2.z;
    float right_2 = spacial_2.pos.x + collision_2.z + collision_2.x;

    return (bottom_1 > top_2 && bottom_2 > top_1 && right_1 > left_2 && right_2 > left_1);
}