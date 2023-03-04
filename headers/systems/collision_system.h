#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "system.h"

#include "spacial.h"
#include "renderable.h"
#include "grid.h"
#include "collision.h"
#include "collider.h"
#include "collidable.h"
#include "interacter.h"
#include "interactable.h"

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