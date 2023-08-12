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
    SpriteSheet& initSpriteSheet(entt::registry& registry, const std::string& sprite_sheet_id);
    SpriteSheet& getSpriteSheet(const std::string& sprite_sheet_id);
private:
    void initAnimations(const std::string& sprite_sheet_id, rapidjson::Document& document);
    void initFrames(entt::registry& registry, const std::string& sprite_sheet_id, rapidjson::Document& document);

    std::tuple<std::string, int> parseFrameName(const std::string& frame_name);
    TextureSource textureSourceFromFrame(const rapidjson::Value& frame, unsigned char* texture_data, glm::ivec2 texture_data_size);

    rapidjson::Document readJSON(const std::string& json_path);

    std::string getTextureSourceKey(const TextureSource& texture_source);

    std::unordered_map<std::string, SpriteSheet> sprite_sheets;

    std::string base_sprite_sheet_path{"./assets/sprite_sheets/"};
};