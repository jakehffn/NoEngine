#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "System.h"

#include "Spacial.h"
#include <glm/glm.hpp>

#include "Collision.h"
#include "Spacial.h"

class CollisionSystem : public System {
public:
    CollisionSystem(entt::registry& registry);

    void update(entt::registry& registry);

    void systemState() override;
private:
    void resolveCollision(Collision collision, Spacial& spacial);

    entt::observer collisionObserver;
};