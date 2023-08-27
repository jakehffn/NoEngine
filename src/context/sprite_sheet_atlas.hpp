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

class SpriteSheetAtlas {
public:
    SpriteSheetAtlas(std::string base_sprite_sheet_path);

    void initMissingTextureSpriteSheet(
        entt::registry& registry, 
        std::string missing_texture_sprite_sheet_id
    );
    SpriteSheet& initSpriteSheet(entt::registry& registry, const std::string& sprite_sheet_id);
    SpriteSheet& getSpriteSheet(const std::string& sprite_sheet_id);
    SpriteSheet& getMissingTextureSpriteSheet();
    std::string_view getBaseSpriteSheetPath() const;
    std::string getSpriteSheetIdFromPath(std::string sprite_sheet_path);
private:
    void initAnimations(const std::string& sprite_sheet_id, rapidjson::Document& document);
    void initFrames(entt::registry& registry, const std::string& sprite_sheet_id, rapidjson::Document& document);

    std::tuple<std::string, int> parseFrameName(const std::string& frame_name);
    std::string parseSpriteSheetName(const std::string& sprite_sheet_id);
    TextureSource textureSourceFromFrame(const rapidjson::Value& frame, unsigned char* texture_data, glm::ivec2 texture_data_size);

    std::optional<rapidjson::Document> readJSON(const std::string& json_path);

    std::string getTextureSourceKey(const TextureSource& texture_source);

    std::unordered_map<std::string, SpriteSheet> sprite_sheets;

    std::string base_sprite_sheet_path;
    std::string missing_texture_sprite_sheet_id;
};