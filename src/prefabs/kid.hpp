#pragma once

#include <entt/entt.hpp>

#include "camera_controller.hpp"
#include "collider.hpp"
#include "collidable.hpp"
#include "idle_animation.hpp"
#include "move_animation.hpp"
#include "player_control.hpp"

void Kid(entt::registry& registry, entt::entity entity) {
    registry.emplace<PlayerControl>(entity);
    registry.emplace<CameraController>(entity);
    registry.emplace<Collider>(entity);

    std::string sprite_sheet_name = "Kid";
    std::string sprite_sheet_id = "characters/kid";

    auto& sprite_sheet_atlas = registry.ctx().at<SpriteSheetAtlas&>();
    auto& sprite_sheet = sprite_sheet_atlas.initSpriteSheet(registry, sprite_sheet_id);
    auto& [default_animation_name, default_animation] = *(sprite_sheet.animations.begin());

    auto& animator = registry.emplace<Animator>(entity, &default_animation.frame_durations);
    registry.emplace<Animation>(entity, &animator, &default_animation);
    registry.emplace<Texture>(entity, sprite_sheet_name, default_animation.frames[0]);
    registry.emplace<IdleAnimation>(entity, registry, sprite_sheet_id, "Idle_Up", "Idle_Down", "Idle_Left", "Idle_Right");
    registry.emplace<MoveAnimation>(entity, registry, sprite_sheet_id, "Move_Up", "Move_Down", "Move_Left", "Move_Right");

    registry.emplace<Renderable>(entity);
}