#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "System.h"

#include "Spacial.h"
#include <glm/glm.hpp>

#include "Collision.h"
#include "Spacial.h"
#include "SpriteState.h"

class CollisionSystem : public System {
public:
    CollisionSystem(entt::registry& registry);

    void update(entt::registry& registry);

    void systemState() override;
private:
    void resolveCollision(SpriteStatePair state, Collision collision, Spacial& spacial, Collision entityCol, Spacial entitySpac);

    entt::observer collisionObserver;
};