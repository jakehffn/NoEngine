#pragma once

#include <unordered_map>

#include "animation.hpp"
#include "spacial.hpp"

#include "sprite_sheet_atlas.hpp"

struct MoveAnimation {
    std::unordered_map<DIRECTION, AnimationData> animations;

    MoveAnimation() {}
    MoveAnimation(
        entt::registry& registry, 
        std::string sprite_sheet_id,
        std::string up_animation_name,
        std::string down_animation_name,
        std::string left_animation_name,
        std::string right_animation_name
    ) {
        auto& sprite_sheet_atlas = registry.ctx().at<SpriteSheetAtlas&>();
        auto& sprite_sheet = sprite_sheet_atlas.getSpriteSheet(sprite_sheet_id);

        this->animations[UP] = sprite_sheet.animations[up_animation_name];
        this->animations[DOWN] = sprite_sheet.animations[down_animation_name];
        this->animations[LEFT] = sprite_sheet.animations[left_animation_name];
        this->animations[RIGHT] = sprite_sheet.animations[right_animation_name];
    }
};