#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "System.h"

#include "Spacial.h"
#include "Collision.h"

class CollisionSystem : public System {
public:
    CollisionSystem(entt::registry& registry);

    void update(entt::registry& registry);

    void systemState() override;
private:
    void resolveCollision(Collision collision, Spacial& spacial);
};