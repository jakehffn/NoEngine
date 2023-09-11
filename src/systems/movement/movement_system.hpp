#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "system.hpp"

#include "velocity.hpp"
#include "spacial.hpp"

#include "clock.hpp"
#include "component_grid.hpp"

#include "debug_timer.hpp"

class MovementSystem : public System {
public:
    MovementSystem(entt::registry& registry);

    void update() override;

private:
    entt::observer velocity_observer;
};