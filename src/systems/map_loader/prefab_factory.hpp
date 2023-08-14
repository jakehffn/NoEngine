#pragma once

#include <entt/entt.hpp>
#include <unordered_map>
#include <string>

#include "kid.hpp"
#include "tree.hpp"

namespace PrefabFactory {
    std::unordered_map<std::string, void (*)(entt::registry&, entt::entity)> prefabs{
        {"Kid", Kid},
        {"Tree", Tree}
    };

    void createIfExists(entt::registry& registry, entt::entity entity, std::string prefab_name) {
        if (prefabs.contains(prefab_name)) {
            prefabs[prefab_name](registry, entity);
        }
    }
}
