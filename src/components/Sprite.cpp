#include "Sprite.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Sprite::Sprite(const char* spritePath, int numSprites) : 
    numSprites{ numSprites }, texData{ glm::vec2(0.0f, 1.0f/numSprites) } {

    // create VAO
    float shaderData[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &this->VAO);
    
    GLuint dataBuffer;
    glGenBuffers(1, &dataBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shaderData), shaderData, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    // Free bound buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);

    // create texture
    unsigned char* textureData = stbi_load(spritePath, &this->width, &this->height, &this->nColorChannels, STBI_rgb_alpha);

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("Created Texture\n");
    } else {
        printf("Failed to load texture\n");
    }

    stbi_image_free(textureData);
}

GLuint Sprite::getVAO() const {
    return this->VAO;
}

GLuint Sprite::getTexture() const {
    // printf("texture retrieved: %i\n", texture);
    return this->texture;
}

int Sprite::getWidth() const {
    return this->width;
}

int Sprite::getHeight() const {
    return this->height;
}