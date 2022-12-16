#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureManager::TextureManager() {}

void TextureManager::initTexture(Texture& texture) {

    if (this->textureMap.count(texture.texturePath) > 0) {
        texture.init(textureMap[texture.texturePath]);
    } else {

        textureMap[texture.texturePath] = this->createTexture(texture);
        texture.init(textureMap[texture.texturePath]);
    }
}

Texture TextureManager::createTexture(Texture texture) {

    texture.texData = glm::vec2(0.0f, 1.0f/texture.numFrames);

    // create texture
    unsigned char* textureData = stbi_load(texture.texturePath.c_str(), &texture.width, &texture.height, &texture.nColorChannels, STBI_rgb_alpha);

    glGenTextures(1, &texture.glTextureID);
    glBindTexture(GL_TEXTURE_2D, texture.glTextureID);

    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture\n");
    }

    stbi_image_free(textureData);

    return texture;
}