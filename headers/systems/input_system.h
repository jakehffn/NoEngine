#pragma once

#include <entt/entt.hpp>

#include "velocity.h"
#include "player_control.h"
#include "spacial.h"

#include "system.h"

#include "input.h"


class InputSystem : public System {
public:
    InputSystem(entt::registry& registry);

    void update() override;

private:
    void playerControlUpdate();

    DIRECTION previousPlayerDirection = DOWN;
};