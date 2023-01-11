#pragma once

#include <entt/entt.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <rapidjson/document.h>

#include "Texture.h"
#include "Spacial.h"

#include "Animation.h"
#include "IdleAnimation.h"
#include "MoveAnimation.h"

#include "AnimationStructs.h"


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

