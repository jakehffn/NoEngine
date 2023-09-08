#pragma once

#include <entt/entt.hpp>

#include "camera_controller.hpp"
#include "collider.hpp"
#include "collidable.hpp"
#include "name.hpp"
#include "renderable.hpp"
#include "idle_animation.hpp"
#include "move_animation.hpp"
#include "player_control.hpp"
#include "velocity.hpp"
#include "state_machine.hpp"
#include "state_machine_builder.hpp"

void TestNpc(entt::registry& registry, entt::entity entity);