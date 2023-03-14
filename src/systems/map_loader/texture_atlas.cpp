#include "texture_atlas.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureAtlas::TextureAtlas(std::string texture_atlas_base_path) : texture_atlas_base_path{ texture_atlas_base_path } {

    this->initTexture();
    this->initSpriteSheets();
}

void TextureAtlas::initTexture() {

    std::string texture_path = texture_atlas_base_path + ".png";
    // create texture
    unsigned char* texture_data = stbi_load(texture_path.c_str(), &(this->width), &(this->height), &(this->num_color_channels), STBI_rgb_alpha);

    glGenTextures(1, &(this->gl_texture_id));
    glBindTexture(GL_TEXTURE_2D, this->gl_texture_id);

    if (texture_data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);

    } else {

        #ifndef NDEBUG
            printf("Failed to load texture\n");
        #endif
    }

    stbi_image_free(texture_data);
}

void TextureAtlas::initSpriteSheets() {

    std::string json;
	std::ifstream json_stream(texture_atlas_base_path + ".json", std::ios::in);

	if(json_stream.is_open()){

		std::stringstream sstr;
		sstr << json_stream.rdbuf();
		json = sstr.str();
		json_stream.close();

	} else {

        #ifndef NDEBUG
		    std::cout << "Unable to open " << texture_atlas_base_path + ".json" << std::endl;
        #endif
	}

    rapidjson::Document document;
    document.Parse(json.c_str());

    #ifndef NDEBUG

        static const char* type_names[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
    
        for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
            itr != document.MemberEnd(); ++itr) {
            printf("Type of member %s is %s\n",
                itr->name.GetString(), type_names[itr->value.GetType()]);
        }

    #endif

    const rapidjson::Value& json_sprite_sheets = document["spritesheets"];
    assert(json_sprite_sheets.IsArray() && "'spritesheets' in json is not an array.");

    for (rapidjson::SizeType x = 0; x < json_sprite_sheets.Size(); x++) {

        const rapidjson::Value& s = json_sprite_sheets[x];

        std::string sprite_sheet_name = s["name"].GetString();

        std::unordered_map<std::string, std::unordered_map<DIRECTION, AnimationData>> new_animations;
        SpriteSheet new_sprite_sheet{
            sprite_sheet_name,
            glm::vec2(s["size"]["w"].GetInt(),s["size"]["h"].GetInt()),
            glm::vec2(s["spriteSize"]["w"].GetInt(),s["spriteSize"]["h"].GetInt()),
        };

        const rapidjson::Value& json_animations = s["animations"];
        assert(json_animations.IsArray() && "'animations' in json is not an array.");

        for (rapidjson::SizeType y = 0; y < json_animations.Size(); y++) {
            
            const rapidjson::Value& a = json_animations[y];

            const rapidjson::Value& json_frames = a["frames"];
            assert(json_animations.IsArray() && "'frames' in json is not an array.");

            std::string name = a["name"].GetString();

            if (name == "") {
                name = "idle";
            }

            std::string direction_string = a["direction"].GetString();
            DIRECTION direction;

            if (direction_string == "up") {
                direction = UP;
            } else if (direction_string == "down") {
                direction = DOWN;
            } else if (direction_string == "left") {
                direction = LEFT;
            } else if (direction_string == "right") {
                direction = RIGHT;
            } else {
                direction = DOWN;
            }

            AnimationData new_animation_data{name, direction, a["numFrames"].GetInt()};

            for (rapidjson::SizeType z = 0; z < json_frames.Size(); z++) {

                const rapidjson::Value& f = json_frames[z];

                new_animation_data.frames.push_back((FrameData) {
                    glm::vec2(f["position"]["x"].GetFloat(), f["position"]["y"].GetFloat()),
                    glm::vec2(f["size"]["w"].GetFloat(), f["size"]["h"].GetFloat()),
                    glm::vec2(f["offset"]["x"].GetFloat(), f["offset"]["y"].GetFloat())
                });

                new_animation_data.frame_durations.push_back(f["duration"].GetFloat());
            }

            new_animations[name][direction] = std::move(new_animation_data);
        }

        new_sprite_sheet.animations = std::move(new_animations);
        this->sprite_sheets[sprite_sheet_name] = std::move(new_sprite_sheet);
    }
}

void TextureAtlas::initEntity(entt::registry& registry, entt::entity entity, std::string sprite_sheet_name) {

    SpriteSheet& sprite_sheet = this->sprite_sheets[sprite_sheet_name];

    AnimationData& default_animation = sprite_sheet.animations["idle"][DOWN];
    auto& texture = registry.emplace_or_replace<Texture>(entity, sprite_sheet_name, 
        (int)sprite_sheet.size.x, (int)sprite_sheet.size.y, &(default_animation.frames[0])
    );

    auto& animator = registry.emplace<Animator>(entity, &(default_animation.frame_durations), default_animation.num_frames);
    registry.emplace_or_replace<Animation>(entity, &animator, &default_animation);

    // TODO: Only entities with multiple frames need animations

    this->addAnimationComponents(registry, entity, sprite_sheet.animations);
}

void TextureAtlas::initTileSet(entt::registry& registry, entt::entity tile_set_entity, std::string tile_set_name) {
    
    SpriteSheet& sprite_sheet = this->sprite_sheets[tile_set_name];

    AnimationData& default_animation = sprite_sheet.animations["idle"][DOWN];
    auto& texture = registry.emplace_or_replace<Texture>(tile_set_entity, tile_set_name, 
        (int)sprite_sheet.size.x, (int)sprite_sheet.size.y, &(default_animation.frames[0])
    );

    // The animator for all tiles in this tileset
    auto& animator = registry.emplace<Animator>(tile_set_entity, &(default_animation.frame_durations));
    // registry.emplace_or_replace<Animation>(entity, &animator);

    // // TODO: Only entities with multiple frames need animations
    // this->addAnimationComponents(registry, entity, sprite_sheet.animations);
}


void TextureAtlas::initTile(entt::registry& registry, entt::entity tile_entity, 
    entt::entity tile_set_entity, glm::vec2 image_position, int tile_gid) {

    auto& animator = registry.get<Animator>(tile_set_entity);
    const auto& tile_set_texture = registry.get<Texture>(tile_set_entity);

    SpriteSheet& sprite_sheet = this->sprite_sheets[tile_set_texture.sprite_sheet_name];
    const AnimationData& base_animation = sprite_sheet.animations["idle"][DOWN];

    if (!this->tile_animation_data.contains(tile_gid)) {
        this->createTileAnimationData(base_animation, image_position, tile_gid);
    } 

    auto& animation = registry.emplace<Animation>(tile_entity, &animator, &(tile_animation_data[tile_gid]));

    auto& texture = registry.emplace<Texture>(tile_entity, tile_set_texture.sprite_sheet_name, 16, 16, &(animation.animation_data->frames[0]));
    
}

void TextureAtlas::createTileAnimationData(const AnimationData& base_animation, const glm::vec2& image_position, int tile_gid) {

    AnimationData new_animation_data{base_animation.name, base_animation.direction, 
        base_animation.num_frames, base_animation.frame_durations};

    for (const FrameData& frame_data : base_animation.frames) {
        new_animation_data.frames.emplace_back(
            glm::vec2(frame_data.position.x + image_position.x,
                frame_data.position.y + image_position.y),
            glm::vec2(16, 16)
        );
    }

    this->tile_animation_data.emplace(std::make_pair(tile_gid, std::move(new_animation_data)));
}

void TextureAtlas::addAnimationComponents(entt::registry& registry, entt::entity entity, AnimationMap& animation_map) {
    
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