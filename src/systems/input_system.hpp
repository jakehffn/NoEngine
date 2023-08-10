#pragma once

#include <entt/entt.hpp>

#include "velocity.hpp"
#include "player_control.hpp"
#include "spacial.hpp"

#include "system.hpp"

#include "input.hpp"


class InputSystem : public System {
public:
    InputSystem(entt::registry& registry);

    void update() override;

private:
    void playerControlUpdate();

    DIRECTION previous_player_direction = DOWN;
};