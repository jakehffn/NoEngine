#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "System.h"

#include "Velocity.h"
#include "Spacial.h"

#include "Clock.h"

class MovementSystem : public System {
public:
    MovementSystem(entt::registry& registry);

    void update() override;

private:
    entt::observer velocityObserver;
};