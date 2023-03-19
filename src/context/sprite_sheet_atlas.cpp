#include "sprite_sheet_atlas.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void SpriteSheetAtlas::initEntity(entt::registry& registry, entt::entity entity, const std::string& sprite_sheet_name) {

    if (!this->sprite_sheets.contains(sprite_sheet_name)) {

        this->initEntitySpriteSheetAseprite(registry, sprite_sheet_name);
    }

    auto& sprite_sheet{this->sprite_sheets[sprite_sheet_name]};

    this->addEntityComponents(registry, entity, sprite_sheet);
    this->addAnimationComponents(registry, entity, sprite_sheet.animations);
}

void SpriteSheetAtlas::beginTileSet(entt::registry& registry, entt::entity entity, const std::string& sprite_sheet_name) {
    
    this->beginTileSetAseprite(registry, entity, sprite_sheet_name);

    auto& sprite_sheet{this->sprite_sheets[sprite_sheet_name]};

    this->addTileSetComponents(registry, entity, sprite_sheet);
    this->addAnimationComponents(registry, entity, sprite_sheet.animations);
}

void SpriteSheetAtlas::endTileSet() {
    this->endTileSetAseprite();
}

void SpriteSheetAtlas::initTile(entt::registry& registry, entt::entity tile_entity, 
    entt::entity tile_set_entity, glm::vec2 image_position, int tile_gid) {

    auto& animator = registry.get<Animator>(tile_set_entity);
    const auto& tile_set_texture = registry.get<Texture>(tile_set_entity);

    SpriteSheet& sprite_sheet = this->sprite_sheets[tile_set_texture.sprite_sheet_name];
    const AnimationData& base_animation = sprite_sheet.animations["idle"][DOWN];

    // If two frames use the same pixel data, then it needs to not be added to the texture atlas a second time
    std::unordered_map<std::string, AtlasData*> frame_map;

    if (!this->tile_animation_data.contains(tile_gid)) {

        this->tile_animation_data[tile_gid] = base_animation;

        auto& tile_set = registry.get<TileSet>(tile_set_entity);

        auto& texture_atlas = registry.ctx().at<TextureAtlas&>();

        for (int frame_num{0}; frame_num < this->current_tile_set_sources.size(); frame_num++) {

            TextureSource reference_source = this->current_tile_set_sources[frame_num];

            // int x_offset{((tile_gid-tile_set.first_gid)*16)%reference_source.size.x};
            // int y_offset{(((tile_gid-tile_set.first_gid)*16)/reference_source.size.x)*16};
            int x_offset{((tile_gid-tile_set.first_gid)*16)%sprite_sheet.sprite_size.x};
            int y_offset{(((tile_gid-tile_set.first_gid)*16)/sprite_sheet.sprite_size.x)*16};

            TextureSource new_texture{
                reference_source.data,
                GL_RGBA,
                glm::ivec2(16, 16),
                glm::ivec2(0),
                reference_source.data_size,
                reference_source.source_offset + 
                    glm::ivec2(x_offset, y_offset)
            };

            std::string key{this->getTextureSourceKey(new_texture)};

            if (!frame_map.contains(key)) {
                frame_map[key] = texture_atlas.insertTexture(new_texture);
            }

            this->tile_animation_data[tile_gid].frames[frame_num] = frame_map[key];
        }
    }

    assert(this->tile_animation_data.contains(tile_gid) && "Tile gid does not exists in tile_animation_data");

    this->addTileComponents(registry, tile_entity, tile_set_texture.sprite_sheet_name, animator);
}

void SpriteSheetAtlas::addEntityComponents(entt::registry& registry, entt::entity entity, SpriteSheet& sprite_sheet) {

    AnimationData& default_animation = sprite_sheet.animations["idle"][DOWN];
    auto& texture = registry.emplace_or_replace<Texture>(
        entity, sprite_sheet.name, default_animation.frames[0]
    );

    auto& animator = registry.emplace<Animator>(entity, &(default_animation.frame_durations), default_animation.num_frames);
    registry.emplace_or_replace<Animation>(entity, &animator, &default_animation);

    // TODO: Only entities with multiple frames need animations
}

void SpriteSheetAtlas::addTileSetComponents(entt::registry& registry, entt::entity entity, SpriteSheet& sprite_sheet) {

    AnimationData& default_animation = sprite_sheet.animations["idle"][DOWN];
    auto& texture = registry.emplace_or_replace<Texture>(
        entity, sprite_sheet.name, default_animation.frames[0]
    );

    // The animator for all tiles in this tileset
    auto& animator = registry.emplace<Animator>(entity, &(default_animation.frame_durations), default_animation.num_frames);
    // registry.emplace_or_replace<Animation>(entity, &animator);

    // // TODO: Only entities with multiple frames need animations
    // this->addAnimationComponents(registry, entity, sprite_sheet.animations);
}

void SpriteSheetAtlas::addTileComponents(entt::registry& registry, entt::entity entity, const std::string& sprite_sheet_name, Animator& animator) {
    
    auto& tile = registry.get<Tile>(entity);

    auto& animation{registry.emplace<Animation>(entity, &animator, &(tile_animation_data[tile.gid]))};
    registry.emplace<Texture>(entity, sprite_sheet_name, animation.animation_data->frames[0]);
}

void SpriteSheetAtlas::addAnimationComponents(entt::registry& registry, entt::entity entity, AnimationMap& animation_map) {
    
    for (auto& [animation_name, animation_direction_map] : animation_map) {
            
        assert(animation_direction_map.contains(DOWN) && "SpriteSheet animation does not include DOWN sprite");

        // Fill any empty animations with the down animation
        if (!animation_direction_map.contains(UP)) {
            animation_direction_map[UP] = animation_direction_map[DOWN];
        }

        if (!animation_direction_map.contains(LEFT)) {
            animation_direction_map[LEFT] = animation_direction_map[DOWN];
        }

        if (!animation_direction_map.contains(RIGHT)) {
            animation_direction_map[RIGHT] = animation_direction_map[DOWN];
        }

        if (animation_name == "idle") {

            registry.emplace_or_replace<IdleAnimation>(entity, animation_direction_map);

        } else if (animation_name == "move") {
            registry.emplace_or_replace<MoveAnimation>(entity, animation_direction_map);
        }
    }
}

// This function does not call updateAtlas()
// TODO: split this into a few more readable functions
void SpriteSheetAtlas::initEntitySpriteSheetAseprite(entt::registry& registry, const std::string& sprite_sheet_name) {

    std::string json_path{this->base_sprite_sheet_path + sprite_sheet_name + ".json"};

    rapidjson::Document document = this->readJSON(json_path);

    const rapidjson::Value& json_meta{document["meta"]};

    // Frames containes all frames of animation for each direction and animation
    const rapidjson::Value& json_frames{document["frames"]};
    assert(json_frames.IsArray() && "'frames' value is not array.");

    SpriteSheet& new_sprite_sheet = this->initNewSpriteSheet(document, sprite_sheet_name);

    auto& texture_atlas = registry.ctx().at<TextureAtlas&>();

    std::string png_path{this->base_sprite_sheet_path + json_meta["image"].GetString()};

    // Loading the sprite_sheet_data
    glm::ivec2 data_size;
    int num_color_channels;
    unsigned char* texture_data = stbi_load(png_path.c_str(), &(data_size.x), &(data_size.y), &(num_color_channels), STBI_rgb_alpha);

    // If two frames use the same pixel data, then it needs to not be added to the texture atlas a second time
    std::unordered_map<std::string, AtlasData*> frame_map;

    // Fill animations with newly-initialized AtlasData
    for (rapidjson::SizeType frame_num{0}; frame_num < json_frames.Size(); frame_num++) {

        const rapidjson::Value& frame = json_frames[frame_num];

        // Filename is a sting that looks like "{sprite_sheet_name}_{animation_name}_{direction}_{frame_number}"
        std::string sprite_frame_name = frame["filename"].GetString();

        auto [animation_name, direction, animation_frame_num] = this->parseFrameName(sprite_frame_name);
        int duration{frame["duration"].GetInt()};

        AnimationData& curr_animation_data{new_sprite_sheet.animations[animation_name][direction]};
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

// TODO: split this into a few more readable functions
void SpriteSheetAtlas::beginTileSetAseprite(entt::registry& registry, entt::entity entity, const std::string& sprite_sheet_name) {

    std::string json_path{this->base_sprite_sheet_path + sprite_sheet_name + ".json"};

    rapidjson::Document document = this->readJSON(json_path);

    const rapidjson::Value& json_meta{document["meta"]};

    // Frames containes all frames of animation for each direction and animation
    const rapidjson::Value& json_frames{document["frames"]};
    assert(json_frames.IsArray() && "'frames' value is not array.");

    SpriteSheet& new_sprite_sheet = this->initNewSpriteSheet(document, sprite_sheet_name);

    auto& texture_atlas = registry.ctx().at<TextureAtlas&>();

    std::string png_path{this->base_sprite_sheet_path + json_meta["image"].GetString()};

    // Loading the sprite_sheet_data
    glm::ivec2 data_size;
    int num_color_channels;
    unsigned char* texture_data = stbi_load(png_path.c_str(), &(data_size.x), &(data_size.y), &(num_color_channels), STBI_rgb_alpha);

    this->current_tile_set_sources.resize(json_frames.Size());

    // Fill animations with newly-initialized AtlasData
    for (rapidjson::SizeType frame_num{0}; frame_num < json_frames.Size(); frame_num++) {

        const rapidjson::Value& frame = json_frames[frame_num];

        // Filename is a sting that looks like "{sprite_sheet_name}_{animation_name}_{direction}_{frame_number}"
        std::string sprite_frame_name = frame["filename"].GetString();

        auto [animation_name, direction, animation_frame_num] = this->parseFrameName(sprite_frame_name);
        int duration{frame["duration"].GetInt()};

        AnimationData& animation_data{new_sprite_sheet.animations["idle"][DOWN]};
        animation_data.frame_durations[animation_frame_num] = duration;

        this->current_tile_set_sources[animation_frame_num] = this->textureSourceFromFrame(frame, texture_data, data_size);
    }
}

void SpriteSheetAtlas::endTileSetAseprite() {
    
    // Cleanup the loaded data from the tileset sources if it exists
    if (this->current_tile_set_sources.size() > 0) {
        stbi_image_free(current_tile_set_sources[0].data);
    }

    this->current_tile_set_sources.clear();
}

rapidjson::Document SpriteSheetAtlas::readJSON(const std::string& json_path) {
    
    std::string json;
	std::ifstream json_stream(json_path, std::ios::in);

	if(json_stream.is_open()){

		std::stringstream sstr;
		sstr << json_stream.rdbuf();
		json = sstr.str();
		json_stream.close();

	} else {

        #ifndef NDEBUG
		    std::cerr << "Unable to open " << json_path << std::endl;
        #endif
	}

    rapidjson::Document document;
    document.Parse(json.c_str());

    #ifndef NDEBUG

        static const char* type_names[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
    
        for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
            itr != document.MemberEnd(); ++itr) {
            std::cout << "Type of member " << itr->name.GetString() << " is " << type_names[itr->value.GetType()] << "\n";
        }

    #endif

    return document;
}

SpriteSheet& SpriteSheetAtlas::initNewSpriteSheet(const rapidjson::Value& document, const std::string& sprite_sheet_name) {

    const rapidjson::Value& json_meta{document["meta"]};

    // Frames containes all frames of animation for each direction and animation
    const rapidjson::Value& json_frames{document["frames"]};
    assert(json_frames.IsArray() && "'frames' value is not array.");

    // Get the sprite_sheet to fill
    this->sprite_sheets[sprite_sheet_name] = {
        sprite_sheet_name,
        glm::vec2(json_meta["size"]["w"].GetInt(),json_meta["size"]["h"].GetInt()),
        glm::vec2(json_frames[0]["sourceSize"]["w"].GetInt(), 
            json_frames[0]["sourceSize"]["h"].GetInt()),
    };

    const rapidjson::Value& animations{json_meta["frameTags"]};
    assert(animations.IsArray() && "'animations' value is not array.");

    const rapidjson::Value& directions{json_meta["layers"]};
    assert(directions.IsArray() && "'directions' value is not array.");

    SpriteSheet& new_sprite_sheet = this->sprite_sheets[sprite_sheet_name];

    // Fill the animation data
    if (animations.Size() == 0) {

        std::string animation_name{"idle"};
        int num_animation_frames{(int)json_frames.Size()};

        for (rapidjson::SizeType direction_num{0}; direction_num < directions.Size(); direction_num++) {

            const rapidjson::Value& current_direction{directions[direction_num]};
            DIRECTION direction{this->formatDirection(current_direction["name"].GetString())};

            new_sprite_sheet.animations[animation_name][direction] = {
                animation_name,
                direction,
                num_animation_frames,
                std::vector<float>(num_animation_frames),
                std::vector<AtlasData*>(num_animation_frames)
            };
        }

    } else {

        for (rapidjson::SizeType animation_num{0}; animation_num < animations.Size(); animation_num++) {

            const rapidjson::Value& current_animation{animations[animation_num]};
            std::string animation_name{this->formatAnimationName(current_animation["name"].GetString())};
            
            int num_animation_frames{current_animation["to"].GetInt() - current_animation["from"].GetInt() + 1};
            
            for (rapidjson::SizeType direction_num{0}; direction_num < directions.Size(); direction_num++) {

                const rapidjson::Value& current_direction{directions[direction_num]};
                DIRECTION direction{this->formatDirection(current_direction["name"].GetString())};

                new_sprite_sheet.animations[animation_name][direction] = {
                    animation_name,
                    direction,
                    num_animation_frames,
                    std::vector<float>(num_animation_frames),
                    std::vector<AtlasData*>(num_animation_frames)
                };
            }
        }
    }

    return new_sprite_sheet;
}

// Parses frame name as tuple of animation name, direction, frame number
std::tuple<std::string, DIRECTION, int> SpriteSheetAtlas::parseFrameName(const std::string& frame_name) {

    // Filename is a string that looks like "{sprite_sheet_name}_{animation_name}_{direction}_{frame_number}"
    
    // The first string is the name of the sprite_sheet which is not needed
    // TODO: removed the first part of the name from the frame name
    auto delimeter_pos{frame_name.find('_')};

    auto substring_start{delimeter_pos+1};
    delimeter_pos = frame_name.find('_', substring_start);

    std::string animation_name{frame_name.substr(substring_start, delimeter_pos-substring_start)};

    animation_name = this->formatAnimationName(animation_name);

    substring_start = delimeter_pos+1;
    delimeter_pos = frame_name.find('_', substring_start);
    std::string direction_string{frame_name.substr(substring_start, delimeter_pos-substring_start)};

    DIRECTION direction{this->formatDirection(direction_string)};

    int animation_frame_num{std::stoi(frame_name.substr(delimeter_pos+1))};

    return std::make_tuple(animation_name, direction, animation_frame_num);
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

std::string SpriteSheetAtlas::formatAnimationName(std::string animation_name) {

    std::transform(animation_name.begin(), animation_name.end(), animation_name.begin(),
        [](unsigned char c){ return std::tolower(c); }
    );

    if (animation_name != "move") {
        animation_name = "idle";
    }

    return animation_name;
}

DIRECTION SpriteSheetAtlas::formatDirection(std::string direction_string) {
    
    std::transform(direction_string.begin(), direction_string.end(), direction_string.begin(),
        [](unsigned char c){ return std::tolower(c); }
    );

    if (direction_string != "up" && direction_string != "left" && direction_string != "right") {
        direction_string = "down";
    }

    return this->direction_string_to_enum[direction_string];
}

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