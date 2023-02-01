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
    TextureAtlas(std::string textureAtlasBasePath);

    void initEntity(entt::registry& registry, entt::entity entity, std::string spriteSheetName);

    int nColorChannels;
    GLuint glTextureID;
    int width;
    int height;

private:

    std::string textureAtlasBasePath;

    void initTexture();
    void initSpriteSheets();

    std::unordered_map<std::string, SpriteSheet> spriteSheets;
};

