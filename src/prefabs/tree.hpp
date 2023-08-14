#pragma once

#include <entt/entt.hpp>

#include "sprite_sheet_atlas.hpp"
#include "collidable.hpp"
#include "renderable.hpp"

void Tree(entt::registry& registry, entt::entity entity) {
    std::string sprite_sheet_name = "NewTree";

    auto& sprite_sheet_atlas = registry.ctx().at<SpriteSheetAtlas&>();
    auto& sprite_sheet = sprite_sheet_atlas.initSpriteSheet(registry, sprite_sheet_name);
    auto& [default_animation_name, default_animation] = *(sprite_sheet.animations.begin());
    registry.emplace<Texture>(entity, sprite_sheet_name, default_animation.frames[0]);
    registry.emplace<Renderable>(entity);
    registry.emplace<Collidable>(entity); 
}