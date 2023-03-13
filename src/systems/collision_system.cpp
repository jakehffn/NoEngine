#include "collision_system.hpp"

CollisionSystem::CollisionSystem(entt::registry& registry) : System(registry),
    collision_observer{ entt::observer(registry, entt::collector.update<Spacial>().where<Collision>()) },
    collider_observer{ entt::observer(registry, entt::collector.update<Spacial>().where<Collision, Collider>()) },
    collidable_observer{ entt::observer(registry, entt::collector.update<Spacial>().where<Collision, Collidable>()) },
    interacter_observer{ entt::observer(registry, entt::collector.update<Spacial>().where<Collision, Interacter>()) },
    interactable_observer{ entt::observer(registry, entt::collector.update<Spacial>().where<Collision, Interactable>()) } {}

void CollisionSystem::update() {

    this->fillAllQueries();

    this->iterateObserverInto<Collidable>(collider_observer, this->resolveCollider);
}

template<typename Component>
void CollisionSystem::iterateObserverInto(entt::observer& observer, 
    void (CollisionSystem::*func)(entt::entity, const glm::vec4&, Spacial&, const glm::vec4&, const Spacial&)) {

        auto entities = registry.view<Collision, Spacial>();

        // Iterate over all entities were moved in the last frame and have Collision
        observer.each([this, entities, func](const auto observed_entity) {

            auto [observed_collision, observed_spacial] = entities.get<Collision, Spacial>(observed_entity);

            // Iterate over the bounding boxes in the observed entity
            for (const auto& observed_bounding_box : observed_collision.bounding_boxes) {
                
                // Iterate over the entities which could be colliding with the observed entity
                for (auto other_entity : observed_collision.current_grid_query) {

                    if (other_entity == observed_entity || !this->registry.all_of<Component>(other_entity)) {
                        continue;
                    }

                    auto [other_collision, other_spacial] = entities.get<Collision, Spacial>(other_entity);

                    for (auto other_bounding_box : other_collision.bounding_boxes) {

                        if (this->isColliding(observed_bounding_box, observed_spacial, other_bounding_box, other_spacial)) {

                            (this->*func)(observed_entity, observed_bounding_box, observed_spacial, 
                                other_bounding_box, other_spacial);
                        }
                    }
                }
            }
        });
}

// Fill the queries of all entities with collision that have moved
void CollisionSystem::fillAllQueries() {

    auto& component_grid = this->registry.ctx().at<ComponentGrid<Renderable, Collision>&>();

    this->collision_observer.each([this, &component_grid](const auto entity) {
        this->registry.patch<Collision>(entity, [this, entity, &component_grid](auto& collision) {

            auto& observed_spacial = this->registry.get<Spacial>(entity);
            
            collision.current_grid_query.clear();

            component_grid.query<Collision>(
                observed_spacial.pos.x, observed_spacial.pos.y,
                observed_spacial.dim.x, observed_spacial.dim.y, collision.current_grid_query);
        });
    });
}

void CollisionSystem::resolveCollider(entt::entity entity, const glm::vec4& collision, Spacial& spacial, 
    const glm::vec4& other_collision, const Spacial& other_spacial) {

        // The epsilon prevent the collision condition from still being true after resolution becuase of 
        //      floating-point error
        // As long as the value resolves to something less than a pixel on the end-user's monitor, 
        //      there will appear to be no bouncing
        float epsilon = 0.005;

        switch (spacial.direction) {
            case UP:
                spacial.pos.y = other_spacial.pos.y + other_collision.w + other_collision.y - collision.w + epsilon;
                break;
            case DOWN:
                spacial.pos.y = other_spacial.pos.y + other_collision.w - collision.y - collision.w - epsilon;
                break;
            case LEFT:
                spacial.pos.x = other_spacial.pos.x + other_collision.z + other_collision.x - collision.z + epsilon;
                break;
            case RIGHT:
                spacial.pos.x = other_spacial.pos.x + other_collision.z - collision.x - collision.z - epsilon;
                break;
        }

        // Patch ensures the component_grid gets the update
        this->registry.patch<Spacial>(entity, [](auto& spacial) {});
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