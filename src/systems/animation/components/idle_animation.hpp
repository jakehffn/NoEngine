#pragma once

#include <unordered_map>

#include "animation.hpp"
#include "spacial.hpp"

#include "sprite_sheet_atlas.hpp"

struct IdleAnimation {
    std::unordered_map<DIRECTION, AnimationData> animations;

    IdleAnimation() {}
    IdleAnimation(
        entt::registry& registry, 
        std::string resource_id,
        std::string up_animation_name,
        std::string down_animation_name,
        std::string left_animation_name,
        std::string right_animation_name
    ) {
        auto& sprite_sheet_atlas = registry.ctx().at<SpriteSheetAtlas&>();
        auto& sprite_sheet = sprite_sheet_atlas.getSpriteSheet(resource_id);
        auto& [_, missing_animation] = *(sprite_sheet_atlas.getMissingTextureSpriteSheet().animations.begin());

        this->animations[UP] = (sprite_sheet.animations.contains(up_animation_name)) ? 
            sprite_sheet.animations[up_animation_name] : missing_animation;
        this->animations[DOWN] = (sprite_sheet.animations.contains(down_animation_name)) ? 
            sprite_sheet.animations[down_animation_name] : missing_animation;
        this->animations[LEFT] = (sprite_sheet.animations.contains(left_animation_name)) ? 
            sprite_sheet.animations[left_animation_name] : missing_animation;
        this->animations[RIGHT] = (sprite_sheet.animations.contains(right_animation_name)) ? 
            sprite_sheet.animations[right_animation_name] : missing_animation;
    }
};