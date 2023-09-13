#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "system.hpp"

#include "spacial.hpp"
#include "renderable.hpp"
#include "component_grid.hpp"
#include "collision.hpp"
#include "collider.hpp"
#include "collidable.hpp"
#include "interaction.hpp"
#include "interactable.hpp"

#include "debug_timer.hpp"

class CollisionSystem : public System {
public:
    CollisionSystem(entt::registry& registry);

    void update() override;

private:

    void fillAllQueries();
    bool isColliding(const glm::vec4& collision_1, const Spacial& spacial_1, const glm::vec4& collision_2, const Spacial& spacial_2);
    template<typename Component>
    void iterateObserverInto(entt::observer& observer, 
        void (CollisionSystem::*func)(entt::entity, const glm::vec4&, Spacial&, const glm::vec4&, const Spacial&));

    void resolveCollider(entt::entity collider_entity, const glm::vec4& collision, Spacial& spacial, 
        const glm::vec4& other_collision, const Spacial& other_spacial);
    
    entt::observer collision_observer;

    entt::observer collider_observer;
    entt::observer collidable_observer;
    entt::observer interacter_observer;
    entt::observer interactable_observer;
};