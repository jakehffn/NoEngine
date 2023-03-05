#pragma once


#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <rapidjson/document.h>

#include "texture.h"
#include "spacial.h"

#include "animation.h"
#include "idle_animation.h"
#include "move_animation.h"

#include "animation_structs.h"


class TextureAtlas {

public:
    TextureAtlas(std::string texture_atlas_base_path);

    void initEntity(entt::registry& registry, entt::entity entity, std::string spriteSheetName);
    void initTileSet(entt::registry& registry, entt::entity tile_set_entity, std::string tile_set_name);
    void initTile(entt::registry& registry, entt::entity tile_entity, 
        entt::entity tile_set_entity, glm::vec2 image_position, int tile_gid);
    
    int num_color_channels;
    GLuint gl_texture_id;
    int width;
    int height;

private:

    void addAnimationComponents(entt::registry& registry, entt::entity entity, AnimationMap& animation_map);

    void createTileAnimationData(const AnimationData& base_animation, const glm::vec2& image_position, int tile_gid);

    void initTexture();
    void initSpriteSheets();

    std::string texture_atlas_base_path;

    // All sprite sheets that were parsed from the json file.
    std::unordered_map<std::string, SpriteSheet> sprite_sheets;
    // GID to AnimationData
    // Tile animations are treated differently becuase of the way they are imported. Tiles are 
    //      always 16x16 and are part of a larger texture, meaninhg that the animations are 
    //      generated outside of the json parsing.
    std::unordered_map<int, AnimationData> tile_animation_data;
};

