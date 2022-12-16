#pragma once

#include <entt/entt.hpp>
#include <unordered_map>
#include <string>

#include "Texture.h"

class TextureManager {

public:
    TextureManager();

    void initTexture(Texture& texture);

private:
    Texture createTexture(Texture texture);

    std::unordered_map<std::string, Texture> textureMap;
};
