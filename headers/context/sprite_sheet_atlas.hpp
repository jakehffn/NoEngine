#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <tuple>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "animation_structs.hpp"
#include "texture_atlas.hpp"

#include "tile.hpp"
#include "tile_set.hpp"

class SpriteSheetAtlas {

public:
    void initEntity(entt::registry& registry, entt::entity entity, const std::string& sprite_sheet_name);
    void beginTileSet(entt::registry& registry, entt::entity entity, const std::string& sprite_sheet_name);
    void initTile(entt::registry& registry, entt::entity tile_entity, entt::entity tile_set_entity, 
        glm::vec2 image_position, int tile_gid);
    
private:

    void addEntityComponents(entt::registry& registry, entt::entity entity, SpriteSheet& sprite_sheet);
    void addTileSetComponents(entt::registry& registry, entt::entity entity, SpriteSheet& sprite_sheet);
    void addTileComponents(entt::registry& registry, entt::entity entity, const std::string& sprite_sheet_name, Animator& animator);
    void addAnimationComponents(entt::registry& registry, entt::entity entity, AnimationMap& animation_map);
    
    void initEntitySpriteSheetAseprite(entt::registry& registry, const std::string& sprite_sheet_name);
    void beginTileSetAseprite(entt::registry& registry, entt::entity entity, const std::string& sprite_sheet_name);
    void endTileSetAseprite();

    rapidjson::Document readJSON(const std::string& json_path);
    SpriteSheet& initNewSpriteSheet(const rapidjson::Value& document, const std::string& sprite_sheet_name);
    std::tuple<std::string, DIRECTION, int> parseFrameName(const std::string& frame_name);
    TextureSource textureSourceFromFrame(const rapidjson::Value& frame, unsigned char* texture_data, glm::ivec2 texture_data_size);

    std::string formatAnimationName(std::string animation_name);
    DIRECTION formatDirection(std::string direction_name);

    std::string getTextureSourceKey(const TextureSource& texture_source);

    // All sprite sheets that were parsed
    std::unordered_map<std::string, SpriteSheet> sprite_sheets;
    // gid to AnimationData
    // Tile animations are treated differently becuase of the way they are imported. Tiles are 
    //      always 16x16 and are part of a larger texture, meaning that the animations are 
    //      generated outside of the json parsing.
    std::vector<TextureSource> current_tile_set_sources;

    std::unordered_map<int, AnimationData> tile_animation_data;
    std::unordered_map<std::string, DIRECTION> direction_string_to_enum{
        {"up", DIRECTION::UP},
        {"down", DIRECTION::DOWN},
        {"left", DIRECTION::LEFT},
        {"right", DIRECTION::RIGHT}
    };
    std::string base_sprite_sheet_path{"./assets/sprite_sheets/"};
};