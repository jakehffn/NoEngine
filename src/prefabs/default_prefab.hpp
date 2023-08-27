#pragma once

#include <filesystem>

#include <entt/entt.hpp>

#include "sprite_sheet_atlas.hpp"
#include "collidable.hpp"
#include "renderable.hpp"

void DefaultPrefab(entt::registry& registry, entt::entity entity, std::string image_path) {
    auto sprite_sheet_path = std::filesystem::path(image_path);
    auto& sprite_sheet_atlas = registry.ctx().at<SpriteSheetAtlas&>();

    auto sprite_sheet_path_partial = sprite_sheet_atlas.getBaseSpriteSheetPath().substr(2);

    std::string sprite_sheet_name = sprite_sheet_path.stem().string();
    std::string sprite_sheet_id = sprite_sheet_atlas.getSpriteSheetIdFromPath(image_path);

    auto& sprite_sheet = sprite_sheet_atlas.initSpriteSheet(registry, sprite_sheet_id);
    auto& [default_animation_name, default_animation] = *(sprite_sheet.animations.begin());
    registry.emplace<Texture>(entity, sprite_sheet_name, default_animation.frames[0]);
    registry.emplace<Renderable>(entity);
    registry.emplace<Collidable>(entity); 
}