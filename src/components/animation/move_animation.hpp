#pragma once

#include <unordered_map>

#include "animation.hpp"
#include "spacial.hpp"

#include "sprite_sheet_atlas.hpp"

struct MoveAnimation {
    std::unordered_map<DIRECTION, AnimationData> animations;

    MoveAnimation() {}
    // Takes 5 arguments:
    // Sprite sheet id and the four animation names for UP, DOWN, LEFT, RIGHT in that order
    MoveAnimation(entt::registry& registry, std::vector<std::string> args) {
        assert(args.size() == 5 && "5 arguments must be provided to MoveAnimation");
        auto& sprite_sheet_atlas = registry.ctx().at<SpriteSheetAtlas&>();
        auto& sprite_sheet_id = args[0];
        auto& sprite_sheet = sprite_sheet_atlas.getSpriteSheet(sprite_sheet_id);

        this->animations[UP] = sprite_sheet.animations[args[1]];
        this->animations[DOWN] = sprite_sheet.animations[args[2]];
        this->animations[LEFT] = sprite_sheet.animations[args[3]];
        this->animations[RIGHT] = sprite_sheet.animations[args[4]];
    }
};