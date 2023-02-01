#pragma once

#include <iostream>
#include <entt/entt.hpp>
#include <unordered_map>
#include <string>
#include <any>

#include "player_control.h"
#include "camera_controller.h"

namespace ComponentFactory {

    void emplaceComponent(entt::registry& registry, entt::entity entity, std::string component, std::vector<std::string> args) {

        if (component == "PlayerControl") {
            registry.emplace_or_replace<PlayerControl>(entity, args);
        } else if (component == "CameraController") {
            registry.emplace_or_replace<CameraController>(entity, args);
        } else {
            std::cout << "No component of type '" << component << "' exists." << std::endl;
        }
    }
}
