#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "system.h"

#include "velocity.h"
#include "spacial.h"

#include "clock.h"
#include "grid.h"

class MovementSystem : public System {
public:
    MovementSystem(entt::registry& registry);

    void update() override;

private:
    entt::observer velocityObserver;
};