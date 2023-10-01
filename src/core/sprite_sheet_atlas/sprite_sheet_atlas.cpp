#include "sprite_sheet_atlas.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void SpriteSheetAtlas::initMissingTextureSpriteSheet(
    entt::registry& registry, 
    std::string missing_texture_sprite_sheet_id) {
        this->missing_texture_sprite_sheet_id = missing_texture_sprite_sheet_id;
        this->initSpriteSheet(registry, missing_texture_sprite_sheet_id);
}

SpriteSheet& SpriteSheetAtlas::initSpriteSheet(entt::registry& registry, const std::string& resource_id) {
    if (this->sprite_sheets.contains(resource_id)) {
        return this->sprite_sheets[resource_id];
    }

    std::string json_path{
        globals::RESOURCE_FOLDER + resource_id + "/" + 
        this->parseSpriteSheetName(resource_id) + ".json"
    };
    auto document = this->readJSON(json_path);

    if (document) {
        this->initAnimations(resource_id, *document);
        this->initFrames(registry, resource_id, *document);
        return this->sprite_sheets[resource_id];
    } else {
        return this->sprite_sheets[this->missing_texture_sprite_sheet_id];
    }
}

SpriteSheet& SpriteSheetAtlas::getSpriteSheet(const std::string& sprite_sheet_id) {
    return this->sprite_sheets[sprite_sheet_id];
}

SpriteSheet& SpriteSheetAtlas::getMissingTextureSpriteSheet() {
    return this->sprite_sheets[missing_texture_sprite_sheet_id];
}

void SpriteSheetAtlas::initAnimations(const std::string& sprite_sheet_id, rapidjson::Document& document) {
    const rapidjson::Value& json_meta{document["meta"]};
    const rapidjson::Value& json_frames{document["frames"]};
    assert(json_frames.IsArray() && "'frames' value is not array.");
    const rapidjson::Value& animations{json_meta["frameTags"]};
    assert(animations.IsArray() && "'animations' value is not array.");
    const rapidjson::Value& states{json_meta["layers"]};
    assert(states.IsArray() && "'states' value is not array.");

    // Get the sprite_sheet to fill
    this->sprite_sheets[sprite_sheet_id] = {
        sprite_sheet_id,
        glm::vec2(
            json_meta["size"]["w"].GetInt(),
            json_meta["size"]["h"].GetInt()
        ),
        glm::vec2(
            json_frames[0]["sourceSize"]["w"].GetInt(), 
            json_frames[0]["sourceSize"]["h"].GetInt()
        ),
    };

    SpriteSheet& new_sprite_sheet = this->sprite_sheets[sprite_sheet_id];

    // Fill the animation data
    if (animations.Size() == 0) {
        int num_animation_frames{(int)json_frames.Size()};

        for (rapidjson::SizeType state_num{0}; state_num < states.Size(); state_num++) {
            const rapidjson::Value& state{states[state_num]};

            new_sprite_sheet.animations[state["name"].GetString()] = {
                "default",
                num_animation_frames,
                std::vector<float>(num_animation_frames),
                std::vector<AtlasData*>(num_animation_frames)
            };
        }
    } else {
        for (rapidjson::SizeType animation_num{0}; animation_num < animations.Size(); animation_num++) {
            const rapidjson::Value& current_animation{animations[animation_num]};
            
            int num_animation_frames{current_animation["to"].GetInt() - current_animation["from"].GetInt() + 1};
            
            for (rapidjson::SizeType state_num{0}; state_num < states.Size(); state_num++) {
                const rapidjson::Value& current_state{states[state_num]};
                std::string animation_name{std::string{current_animation["name"].GetString()} + "_" + std::string{current_state["name"].GetString()}};

                new_sprite_sheet.animations[animation_name] = {
                    animation_name,
                    num_animation_frames,
                    std::vector<float>(num_animation_frames),
                    std::vector<AtlasData*>(num_animation_frames)
                };
            }
        }
    }
}

void SpriteSheetAtlas::initFrames(entt::registry& registry, const std::string& resource_id, rapidjson::Document& document) {
    auto& texture_atlas = registry.ctx().at<TextureAtlas&>();
    
    const rapidjson::Value& json_meta{document["meta"]};
    const rapidjson::Value& json_frames{document["frames"]};
    assert(json_frames.IsArray() && "'frames' value is not array.");
    const rapidjson::Value& animations{json_meta["frameTags"]};
    assert(animations.IsArray() && "'animations' value is not array.");
    const rapidjson::Value& states{json_meta["layers"]};
    assert(states.IsArray() && "'states' value is not array.");

    std::string png_path{
        globals::RESOURCE_FOLDER + resource_id + "/" + 
        this->parseSpriteSheetName(resource_id) + ".png"
    };
    SpriteSheet& new_sprite_sheet = this->sprite_sheets[resource_id];
    
    // Loading the sprite_sheet_data
    glm::ivec2 data_size;
    int num_color_channels;
    unsigned char* texture_data = stbi_load(png_path.c_str(), &(data_size.x), &(data_size.y), &(num_color_channels), STBI_rgb_alpha);
    // If two frames use the same pixel data, then it needs to not be added to the texture atlas a second time
    std::unordered_map<std::string, AtlasData*> frame_map;

    // Fill animations with newly-initialized AtlasData
    for (rapidjson::SizeType frame_num{0}; frame_num < json_frames.Size(); frame_num++) {
        const rapidjson::Value& frame = json_frames[frame_num];

        // Filename is a string that looks like "{sprite_sheet_name}_{animation_name}_{direction}_{frame_number}"
        std::string sprite_frame_name = frame["filename"].GetString();

        auto [animation_name, animation_frame_num] = this->parseFrameName(sprite_frame_name);
        int duration{frame["duration"].GetInt()};

        AnimationData& curr_animation_data{new_sprite_sheet.animations[animation_name]};
        curr_animation_data.frame_durations[animation_frame_num] = duration;

        TextureSource new_texture{this->textureSourceFromFrame(frame, texture_data, data_size)};

        AtlasData* atlas_data;
        std::string key{this->getTextureSourceKey(new_texture)};

        if (!frame_map.contains(key)) {
            frame_map[key] = texture_atlas.insertTexture(new_texture);
        }
        curr_animation_data.frames[animation_frame_num] = frame_map[key];
    }
    stbi_image_free(texture_data);
}

std::optional<rapidjson::Document> SpriteSheetAtlas::readJSON(const std::string& json_path) {
    std::string json;
	std::ifstream json_stream(json_path, std::ios::in);

    std::optional<rapidjson::Document> document;

	if(json_stream.is_open()){
		std::stringstream sstr;
		sstr << json_stream.rdbuf();
		json = sstr.str();
		json_stream.close();

        document = rapidjson::Document();
        document->Parse(json.c_str());

	} else {
        #ifndef NDEBUG
		    std::cerr << "Unable to open " << json_path << std::endl;
        #endif
	}

    return document;
}

// Parses frame name as tuple of animation name, direction, frame number
std::tuple<std::string, int> SpriteSheetAtlas::parseFrameName(const std::string& frame_name) {
    // Filename is a string that looks like "{sprite_sheet_name}_{animation_name}_{state}_{frame_number}"
    // NOTE: If needed, the aseprite format is actually {title}_{tag}_{layer}_{frame}
    
    // The first string is the name of the sprite_sheet which is not needed
    // TODO: removed the first part of the name from the frame name
    auto delimeter_pos{frame_name.find('_')};

    auto substring_start{delimeter_pos+1};
    delimeter_pos = frame_name.find('_', substring_start);

    std::string animation_name{frame_name.substr(substring_start, delimeter_pos-substring_start)};

    substring_start = delimeter_pos+1;
    delimeter_pos = frame_name.find('_', substring_start);
    std::string state{frame_name.substr(substring_start, delimeter_pos-substring_start)};

    int animation_frame_num{std::stoi(frame_name.substr(delimeter_pos+1))};

    std::string full_animation_name;
    if (animation_name != "") {
        full_animation_name = animation_name + "_" + state;
    } else {
        full_animation_name = state;
    }
    return std::make_tuple(full_animation_name, animation_frame_num);
}

std::string SpriteSheetAtlas::parseSpriteSheetName(const std::string& resource_id) {
    return resource_id.substr(resource_id.find_last_of('/') + 1);
}

TextureSource SpriteSheetAtlas::textureSourceFromFrame(const rapidjson::Value& frame, unsigned char* texture_data, glm::ivec2 texture_data_size) {
    // Get sprite data
    const rapidjson::Value& sprite_info{frame["spriteSourceSize"]};
    glm::ivec2 size(sprite_info["w"].GetInt(), sprite_info["h"].GetInt());
    glm::ivec2 offset(sprite_info["x"].GetInt(), sprite_info["y"].GetInt());

    const rapidjson::Value& frame_pos_in_sprite_sheet{frame["frame"]};
    glm::ivec2 source_offset(frame_pos_in_sprite_sheet["x"].GetInt(), 
        frame_pos_in_sprite_sheet["y"].GetInt());

    return {
        texture_data,
        GL_RGBA,
        size,
        offset,
        texture_data_size,
        source_offset
    };
}

// Generates simple key which represents a unique texture source
std::string SpriteSheetAtlas::getTextureSourceKey(const TextureSource& texture_source) {

    std::string key_offset{
        std::to_string(texture_source.source_offset.x)+
        "_"+
        std::to_string(texture_source.source_offset.y)
    };
    std::string key_size{
        std::to_string(texture_source.size.x)+
        "_"+
        std::to_string(texture_source.size.y)
    };

    return {key_offset+"_"+key_size};
}