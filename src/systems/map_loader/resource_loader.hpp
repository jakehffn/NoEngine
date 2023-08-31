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

namespace ResourceLoader {
    std::unordered_map<std::string, void (*)(entt::registry&, entt::entity)> prefabs{
        {"Kid", Kid},
        {"TestNpc", TestNpc}
    };

    bool create(entt::registry& registry, entt::entity entity, std::string prefab_name) {
        auto prefab_exists = prefabs.contains(prefab_name);
        if (prefab_exists) {
            prefabs[prefab_name](registry, entity);
        }
        return prefab_exists;
    }

    // A resource is a folder of related files for an entity
    // The resource id is the portion of the path of the folder relative to the base assets folder
    // Eg. for the sprite at "C:Users/.../assets/characters/kid/kid.png", resource id would be "characters/kid"
    std::string getResourceIdFromSpecificPath(std::string specific_resource_path) {
        auto resource_folder_nonrelative = constant::RESOURCE_FOLDER.substr(2);

        size_t resource_id_start = specific_resource_path.find(resource_folder_nonrelative) + 
            resource_folder_nonrelative.length();

        return specific_resource_path.substr(
            resource_id_start,
            specific_resource_path.find_last_of('/') - resource_id_start
        );
    }

    void createDefault(entt::registry& registry, entt::entity entity, std::string image_path) {
        auto sprite_sheet_path = std::filesystem::path(image_path);
        auto& sprite_sheet_atlas = registry.ctx().at<SpriteSheetAtlas&>();

        std::string sprite_sheet_name = sprite_sheet_path.stem().string();
        std::string resource_id = getResourceIdFromSpecificPath(image_path);

        auto& sprite_sheet = sprite_sheet_atlas.initSpriteSheet(registry, resource_id);
        auto& [default_animation_name, default_animation] = *(sprite_sheet.animations.begin());
        registry.emplace<Texture>(entity, sprite_sheet_name, default_animation.frames[0]);

        if (default_animation.num_frames > 1) {
            auto& animator = registry.emplace<Animator>(entity, &default_animation.frame_durations);
            registry.emplace<Animation>(entity, &animator, &default_animation);
        }

        registry.emplace<Renderable>(entity);
        registry.emplace<Collidable>(entity); 
        registry.emplace<Name>(entity, resource_id);
    }
}
