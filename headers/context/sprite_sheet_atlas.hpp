#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "animation_structs.hpp"
#include "texture_atlas.hpp"

#include "tile.hpp"
#include "tile_set.hpp"

class SpriteSheetAtlas {

public:
    void initEntity(entt::registry& registry, entt::entity entity, std::string sprite_sheet_name);
    void initTileSet(entt::registry& registry, entt::entity entity, std::string sprite_sheet_name);
    void initTile(entt::registry& registry, entt::entity tile_entity, entt::entity tile_set_entity, 
        glm::vec2 image_position, int tile_gid);
    
private:
    void addEntityComponents(entt::registry& registry, entt::entity entity, SpriteSheet& sprite_sheet);
    void addTileSetComponents(entt::registry& registry, entt::entity entity, SpriteSheet& sprite_sheet);
    void addTileComponents(entt::registry& registry, entt::entity entity, std::string sprite_sheet_name, Animator& animator);
    void addAnimationComponents(entt::registry& registry, entt::entity entity, AnimationMap& animation_map);
    
    void initEntitySpriteSheetAseprite(entt::registry& registry, std::string sprite_sheet_name);
    void initTileSetSpriteSheetAseprite(entt::registry& registry, entt::entity entity, std::string sprite_sheet_name);

    // TextureSource getFrameTextureSource(const rapidjson::Value& frame_json, unsigned char* data);

    rapidjson::Document readJSON(std::string json_path);

    // All sprite sheets that were parsed
    std::unordered_map<std::string, SpriteSheet> sprite_sheets;
    // gid to AnimationData
    // Tile animations are treated differently becuase of the way they are imported. Tiles are 
    //      always 16x16 and are part of a larger texture, meaning that the animations are 
    //      generated outside of the json parsing.
    std::unordered_map<int, AnimationData> tile_animation_data;
    std::unordered_map<std::string, DIRECTION> direction_string_to_enum{
        {"Up", DIRECTION::UP},
        {"Down", DIRECTION::DOWN},
        {"Left", DIRECTION::LEFT},
        {"Right", DIRECTION::RIGHT}
    };
    std::string base_sprite_sheet_path{"./assets/sprite_sheets/"};
};