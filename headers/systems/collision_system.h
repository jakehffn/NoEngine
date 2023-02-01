#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "system.h"

#include "spacial.h"
#include "collision.h"

class CollisionSystem : public System {
public:
    CollisionSystem(entt::registry& registry);

    void update() override;

private:
    void resolveCollision(glm::vec4 collision, Spacial& spacial, glm::vec4 entityCol, Spacial entitySpac);

    entt::observer collisionObserver;
};