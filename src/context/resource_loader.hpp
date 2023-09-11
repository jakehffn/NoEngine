#pragma once

#include <filesystem>
#include <unordered_map>
#include <string>

#include <entt/entt.hpp>

#include "consts.hpp"
#include "renderable.hpp"
#include "name.hpp"

#include "kid.hpp"
#include "test_npc.hpp"
#include "custom_cursor.hpp"
#include "fps_counter_entity.hpp"

namespace ResourceLoader {
    static std::unordered_map<std::string, void (*)(entt::registry&, entt::entity)> prefabs{
        {"Kid", Kid},
        {"TestNpc", TestNpc},
        {"CustomCursor", CustomCursor},
        {"FpsCounterEntity", FpsCounterEntity}
    };

    bool create(entt::registry& registry, entt::entity entity, std::string prefab_name);

    // A resource is a folder of related files for an entity
    // The resource id is the portion of the path of the folder relative to the base assets folder
    // Eg. for the sprite at "C:Users/.../assets/characters/kid/kid.png", resource id would be "characters/kid"
    std::string getResourceIdFromSpecificPath(std::string specific_resource_path);

    void createDefault(entt::registry& registry, entt::entity entity, std::string image_path);
}
