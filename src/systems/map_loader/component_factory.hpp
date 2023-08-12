#pragma once

#include <iostream>
#include <entt/entt.hpp>
#include <unordered_map>
#include <string>
#include <any>

#include "player_control.hpp"
#include "camera_controller.hpp"
#include "collider.hpp"
#include "collidable.hpp"
#include "idle_animation.hpp"
#include "move_animation.hpp"

#define WITH_COMPONENTS(F) \
    F(PlayerControl, args) \
    F(CameraController, args) \
    F(Collider) \
    F(Collidable) \
    F(IdleAnimation, registry, args) \
    F(MoveAnimation, registry, args) 

#define EMPLACE_COMPONENTS(c, ...) if (component == #c) { \
        registry.emplace_or_replace<c>(entity __VA_OPT__(,) __VA_ARGS__); \
    } else

namespace ComponentFactory {
    void emplaceComponent(entt::registry& registry, entt::entity entity, std::string component, std::vector<std::string> args) {
        WITH_COMPONENTS(EMPLACE_COMPONENTS) {
            #ifndef NDEBUG
                std::cerr << "No component of type '" << component << "' exists." << std::endl;
            #endif
        }
    }
}
