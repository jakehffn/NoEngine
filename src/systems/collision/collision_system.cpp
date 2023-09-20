#include "collision_system.hpp"

CollisionSystem::CollisionSystem(entt::registry& registry) : System(registry),
    collision_observer{ entt::observer(registry, entt::collector.update<Spacial>().where<Collision, GridData<Collision>>()) },
    collider_observer{ entt::observer(registry, entt::collector.update<Collision>().where<Spacial, Collider>()) } {}

void CollisionSystem::update() {
    DEBUG_TIMER(_, "CollisionSystem::update");
    this->fillCollisions();
    this->resolveCollisions();
}
#include <iostream>
// Fill the queries of all entities with collision that have moved
void CollisionSystem::fillCollisions() {
    auto& component_grid = this->registry.ctx().at<ComponentGrid<Collision>&>();
    std::vector<entt::entity> query_results;

    this->collision_observer.each([this, &component_grid, &query_results](const auto entity) {
        auto [spacial, collision, grid_data] = this->registry.get<Spacial, Collision, GridData<Collision>>(entity);
        
        query_results.clear();
        component_grid.query(
            grid_data.bounds,
            query_results
        );

        collision.collisions.clear();

        for (auto other_entity : query_results) {
            auto [other_collision, other_spacial] = this->registry.get<Collision, Spacial>(other_entity);

            for (auto bounding_box : collision.bounding_boxes) {
                for (auto other_bounding_box : other_collision.bounding_boxes) {
                    if (entity != other_entity && this->isColliding(bounding_box, spacial, other_bounding_box, other_spacial)) {
                        collision.collisions.push_back(other_entity);
                    }
                }
            }
        }

        if (collision.collisions.size() > 0) {
            // Allow for updates on collision by other systems when a collision occurs
            this->registry.patch<Collision>(entity);
        }
    });
}

void CollisionSystem::resolveCollisions() {
    for (auto entity : this->collider_observer) {
        auto [collision, spacial] = this->registry.get<Collision, Spacial>(entity);

        for (auto other_entity : collision.collisions) {
            if (!this->registry.valid(other_entity) || !this->registry.all_of<Collidable>(other_entity)) {
                continue;
            }

            auto [other_collision, other_spacial] = this->registry.get<Collision, Spacial>(other_entity);

            for (auto bounding_box : collision.bounding_boxes) {
                for (auto other_bounding_box : other_collision.bounding_boxes) {

                    if (this->isColliding(bounding_box, spacial, other_bounding_box, other_spacial)) {
                        this->resolveCollision(
                            entity, 
                            bounding_box, 
                            spacial, 
                            other_bounding_box, 
                            other_spacial
                        );
                    }
                }
            }
        }
    }
}

void CollisionSystem::resolveCollision(entt::entity entity, const glm::vec4& collision, Spacial& spacial, 
    const glm::vec4& other_collision, const Spacial& other_spacial) {

        // The epsilon prevent the collision condition from still being true after resolution becuase of 
        //      floating-point error
        // As long as the value resolves to something less than a pixel on the end-user's monitor, 
        //      there will appear to be no bouncing
        float epsilon = 0.005;

        switch (spacial.direction) {
            case UP:
                spacial.position.y = other_spacial.position.y + other_collision.w + other_collision.y - collision.w + epsilon;
                break;
            case DOWN:
                spacial.position.y = other_spacial.position.y + other_collision.w - collision.y - collision.w - epsilon;
                break;
            case LEFT:
                spacial.position.x = other_spacial.position.x + other_collision.z + other_collision.x - collision.z + epsilon;
                break;
            case RIGHT:
                spacial.position.x = other_spacial.position.x + other_collision.z - collision.x - collision.z - epsilon;
                break;
        }

        // Patch ensures the component_grid gets the update
        this->registry.patch<Spacial>(entity, [](auto& spacial) {});
}

bool CollisionSystem::isColliding(const glm::vec4& collision_1, const Spacial& spacial_1, const glm::vec4& collision_2, const Spacial& spacial_2) {
    float top_1 = spacial_1.position.y + collision_1.w;
    float bottom_1 = spacial_1.position.y + collision_1.w + collision_1.y;
    float left_1 = spacial_1.position.x + collision_1.z;
    float right_1 = spacial_1.position.x + collision_1.z + collision_1.x;

    float top_2 = spacial_2.position.y + collision_2.w;
    float bottom_2 = spacial_2.position.y + collision_2.w + collision_2.y;
    float left_2 = spacial_2.position.x + collision_2.z;
    float right_2 = spacial_2.position.x + collision_2.z + collision_2.x;

    return (bottom_1 > top_2 && bottom_2 > top_1 && right_1 > left_2 && right_2 > left_1);
}