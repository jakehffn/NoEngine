#pragma once

#include <filesystem>

#include <entt/entt.hpp>

#include "camera_controller.hpp"
#include "collider.hpp"
#include "collidable.hpp"
#include "player_control.hpp"
#include "name.hpp"
#include "renderable.hpp"
#include "sprite_sheet_atlas.hpp"
#include "component_grid_ignore.hpp"
#include "gui_element.hpp"

static void CustomCursor(entt::registry& registry, entt::entity entity) {
    auto& sprite_sheet_atlas = registry.ctx().at<SpriteSheetAtlas&>();

    std::string name = "Cursor";
    std::string resource_id = "misc/PinkFlower";

    auto& sprite_sheet = sprite_sheet_atlas.initSpriteSheet(registry, resource_id);
    auto& [default_animation_name, default_animation] = *(sprite_sheet.animations.begin());
    registry.emplace<Texture>(entity, name, default_animation.frames[0]);

    if (default_animation.num_frames > 1) {
        auto& animator = registry.emplace<Animator>(entity, &default_animation.frame_durations);
        registry.emplace<Animation>(entity, &animator, &default_animation);
    }

    registry.emplace<ComponentGridIgnore>(entity);
    registry.emplace<Renderable>(entity);
    registry.emplace<Collidable>(entity); 
    registry.emplace<Name>(entity, resource_id);
    registry.emplace<Spacial>(entity);

    registry.emplace<GuiElement>(entity);
}