#include "TextureAtlas.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureAtlas::TextureAtlas(std::string textureAtlasBasePath) : textureAtlasBasePath{ textureAtlasBasePath } {

    this->initTexture();
    this->initSpriteSheets();
}

void TextureAtlas::initTexture() {

    std::string texturePath = textureAtlasBasePath + ".png";
    // create texture
    unsigned char* textureData = stbi_load(texturePath.c_str(), &(this->width), &(this->height), &(this->nColorChannels), STBI_rgb_alpha);

    glGenTextures(1, &(this->glTextureID));
    glBindTexture(GL_TEXTURE_2D, this->glTextureID);

    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture\n");
    }

    stbi_image_free(textureData);
}

void TextureAtlas::initSpriteSheets() {

    std::string json;
	std::ifstream jsonStream(textureAtlasBasePath + ".json", std::ios::in);

	if(jsonStream.is_open()){

		std::stringstream sstr;
		sstr << jsonStream.rdbuf();
		json = sstr.str();
		jsonStream.close();

	}else{
		std::cout << "Unable to open " << textureAtlasBasePath + ".json" << std::endl;
	}

    rapidjson::Document document;
    document.Parse(json.c_str());

    static const char* kTypeNames[] = 
    { "Null", "False", "True", "Object", "Array", "String", "Number" };
 
    for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
        itr != document.MemberEnd(); ++itr)
    {
        printf("Type of member %s is %s\n",
            itr->name.GetString(), kTypeNames[itr->value.GetType()]);
    }

    const rapidjson::Value& jsonSpriteSheets = document["spritesheets"];
    assert(jsonSpriteSheets.IsArray() && "'spritesheets' in json is not an array.");

    for (rapidjson::SizeType x = 0; x < jsonSpriteSheets.Size(); x++) {

        const rapidjson::Value& s = jsonSpriteSheets[x];

        SpriteSheet newSheet = {
            glm::vec2(s["size"]["w"].GetInt(),s["size"]["h"].GetInt()),
            glm::vec2(s["spriteSize"]["w"].GetInt(),s["spriteSize"]["h"].GetInt())
        };

        const rapidjson::Value& jsonAnimations = s["animations"];
        assert(jsonAnimations.IsArray() && "'animations' in json is not an array.");

        for (rapidjson::SizeType y = 0; y < jsonAnimations.Size(); y++) {
            
            const rapidjson::Value& a = jsonAnimations[y];

            const rapidjson::Value& jsonFrames = a["frames"];
            assert(jsonAnimations.IsArray() && "'frames' in json is not an array.");

            std::string name = a["name"].GetString();

            if (name == "") {
                name == "idle";
            }

            std::string directionString = a["direction"].GetString();
            DIRECTION direction;

            if (directionString == "up") {
                direction = UP;
            } else if (directionString == "down") {
                direction = DOWN;
            } else if (directionString == "left") {
                direction = LEFT;
            } else if (directionString == "right") {
                direction = RIGHT;
            } else {
                direction = DOWN;
            }

            AnimationData newAnimation = {name, direction, a["numFrames"].GetInt()};

            for (rapidjson::SizeType z = 0; z < jsonFrames.Size(); z++) {

                const rapidjson::Value& f = jsonFrames[z];

                newAnimation.frames.push_back((FrameData) {
                    f["duration"].GetFloat(),
                    glm::vec2(f["position"]["x"].GetFloat(), f["position"]["y"].GetFloat()),
                    glm::vec2(f["size"]["w"].GetFloat(), f["size"]["h"].GetFloat()),
                    glm::vec2(f["offset"]["x"].GetFloat(), f["offset"]["y"].GetFloat())
                });
            }

            newSheet.animations[name][direction] = newAnimation;
        }

        this->spriteSheets[s["name"].GetString()] = newSheet;
    }
}

void TextureAtlas::initEntity(entt::registry& registry, entt::entity entity, std::string spriteSheetName) {

    SpriteSheet& spriteSheet = this->spriteSheets[spriteSheetName];

    auto& texture = registry.emplace_or_replace<Texture>(entity, spriteSheetName, (int)spriteSheet.size.x, (int)spriteSheet.size.y, spriteSheet.animations["idle"][DOWN].frames[0]);

    registry.emplace_or_replace<Animation>(entity);

    // TODO: Only entities with multiple frames need animations

    for (auto& [animationName, animationMap] : spriteSheet.animations) {

        if (animationName == "idle") {

            registry.emplace_or_replace<IdleAnimation>(entity, animationMap);

            assert(animationMap.find(DOWN) != animationMap.end() && "SpriteSheet idle animation does not include DOWN sprite");

            texture.frameData = animationMap[DOWN].frames[0];

        } else if (animationName == "move") {
            registry.emplace_or_replace<MoveAnimation>(entity, animationMap);
        }
    }
}