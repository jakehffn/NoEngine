#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "system.h"

#include "spacial.h"
#include "collision.h"
#include "Renderable.h"
#include "grid.h"

class CollisionSystem : public System {
public:
    CollisionSystem(entt::registry& registry);

    void update() override;

private:
    void resolveCollision(const glm::vec4& collision, Spacial& spacial, const glm::vec4& entityCol, const Spacial& entitySpac);
    bool isColliding(const glm::vec4& collision_1, const Spacial& spacial_1, const glm::vec4& collision_2, const Spacial& spacial_2);
    
    entt::observer collision_observer;
    std::vector<entt::entity> collision_query;
};