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

    registry.emplace<IdleAnimation>(entity, registry, "Kid", "Idle_Up", "Idle_Down", "Idle_Left", "Idle_Right");
    registry.emplace<MoveAnimation>(entity, registry, "Kid", "Move_Up", "Move_Down", "Move_Left", "Move_Right");
    
    auto& sprite_sheet_atlas = registry.ctx().at<SpriteSheetAtlas&>();
    auto name = registry.get<Name>(entity);
    auto& sprite_sheet = sprite_sheet_atlas.getSpriteSheet(name.name);
    auto& [default_animation_name, default_animation] = *(sprite_sheet.animations.begin());
    auto& animator = registry.emplace<Animator>(entity, &default_animation.frame_durations);
    registry.emplace<Animation>(entity, &animator, &default_animation);

    registry.remove<Collidable>(entity); 
}