#pragma once

#include <entt/entt.hpp>
#include <component_grid.hpp>

#include "velocity.hpp"
#include "player_control.hpp"
#include "spacial.hpp"
#include "outline.hpp"
#include "camera.hpp"
#include "renderable.hpp"
#include "cursor.hpp"
#include "left_clicked.hpp"
#include "right_clicked.hpp"

#include "system.hpp"

#include "input.hpp"
#include "resource_loader.hpp"

#include "debug_timer.hpp"

class InputSystem : public System {
public:
    InputSystem(entt::registry& registry);

    void update() override;

private:
    void playerControlUpdate();
    void selectedUpdate();

    DIRECTION previous_player_direction{DOWN};
    entt::entity cursor_entity;
};