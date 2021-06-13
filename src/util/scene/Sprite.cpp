#include "Sprite.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Sprite::Sprite(char* spritePath) {

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
    
    glGenVertexArrays(1, &(this->VAO));
    glBindVertexArray(this->VAO);

    GLuint dataBuffer;
    glGenBuffers(1, &dataBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shaderData), shaderData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);

    // create texture
    unsigned char* textureData = stbi_load(spritePath, &this->width, &this->height, &this->nColorChannels, 0);

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("Created Texture");
    } else {
        printf("Failed to load texture");
    }

    stbi_image_free(textureData);
}

GLuint Sprite::getVAO() {
    return this->VAO;
}

GLuint Sprite::getTexture() {
    printf("texture retrieved");
    return this->texture;
}