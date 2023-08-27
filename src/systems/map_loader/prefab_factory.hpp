#pragma once

#include <entt/entt.hpp>
#include <unordered_map>
#include <string>

#include "kid.hpp"
#include "default_prefab.hpp"

namespace PrefabFactory {
    std::unordered_map<std::string, void (*)(entt::registry&, entt::entity)> prefabs{
        {"Kid", Kid}
    };

    bool create(entt::registry& registry, entt::entity entity, std::string prefab_name) {
        auto prefab_exists = prefabs.contains(prefab_name);
        if (prefab_exists) {
            prefabs[prefab_name](registry, entity);
        }
        return prefab_exists;
    }

    void createDefault(entt::registry& registry, entt::entity entity, std::string image_path) {
        DefaultPrefab(registry, entity, image_path);
    }
}
