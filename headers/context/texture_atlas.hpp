#pragma once

#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <rapidjson/document.h>
#include <retpack2d/finders_interface.h>

#include "texture.hpp"
#include "spacial.hpp"

#include "animation.hpp"
#include "idle_animation.hpp"
#include "move_animation.hpp"

#include "atlas_data.hpp"

struct TextureSource {
    unsigned char* data;
    GLuint data_format;
    glm::ivec2 size; // Width, height of resulting texture
    glm::ivec2 offset; // Offset for use by renderer
    glm::ivec2 data_size; // Size of the source data
    glm::ivec2 source_offset; // Offset from source data begin
};

class TextureAtlas {

public:
    TextureAtlas();

    AtlasData* insertTexture(const TextureSource& source);
    void updateAtlas();

    int num_color_channels;
    GLuint gl_texture_id;
    int width;
    int height;

private:
    struct TextureSourceData {
        int source_index;
        AtlasData* atlas_data;
    };

    void updateAtlasDataPacking();
    void updateAtlasTexture();
    // void copyDataToAtlasSource(unsigned char* data, glm::ivec2 size, glm::ivec2 position);

    std::vector<TextureSourceData> sources_data;
    std::vector<std::vector<unsigned char>> sources;
    std::list<AtlasData> atlas_data; // atlas_data needs to be list for pointer stability
};

