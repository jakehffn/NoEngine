#include "renderer.h"

Renderer::Renderer() : screenShader{ new ScreenShader() }, 
    instancedShader{ new InstancedShader() } {

        glClearColor(0.0f, 0.4f, 0.4f, 0.0f);
        glEnable(GL_BLEND); 
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        this->initVAO();
        this->initScreenFBO();
        this->initVBOs();
}

Renderer::~Renderer() {
    glDeleteFramebuffers(1, &(this->screenFBO));  
}

void Renderer::initVAO() {

    // create VAO
    float quadVertexData[] = { 
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
    
    // The verticies will never change so the buffer ID is not saved
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // Free bound buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
}

void Renderer::initVBOs() {

    glGenBuffers(1, &(this->textureCoordinatesVBO));
    glGenBuffers(1, &(this->modelsVBO));

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->textureCoordinatesBufferData.size(), this->textureCoordinatesBufferData.data(), GL_STREAM_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glVertexAttribDivisor(1, 1); 

    glBindBuffer(GL_ARRAY_BUFFER, this->modelsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * this->modelsBufferData.size(), this->textureCoordinatesBufferData.data(), GL_STREAM_DRAW);

    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(4); 
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5); 
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6); 
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
}

void Renderer::initScreenFBO() {

    glGenFramebuffers(1, &(this->screenFBO));
    glBindFramebuffer(GL_FRAMEBUFFER, this->screenFBO);

    glGenTextures(1, &(this->screenTexture));
    glBindTexture(GL_TEXTURE_2D, this->screenTexture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_c::SCREEN_WIDTH, render_c::SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->screenTexture, 0); 

    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void Renderer::addBufferData(const glm::vec4& textureData, const glm::mat4& modelData) {

    this->textureCoordinatesBufferData.push_back(textureData);
    this->modelsBufferData.push_back(modelData);
}

void Renderer::bufferData() {

    // glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*this->textureCoordinatesBufferData.size(), NULL, GL_STREAM_DRAW);

    // glBindBuffer(GL_ARRAY_BUFFER, this->modelsVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*this->modelsBufferData.size(), NULL, GL_STREAM_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*this->textureCoordinatesBufferData.size(), this->textureCoordinatesBufferData.data(), GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->modelsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*this->modelsBufferData.size(), this->modelsBufferData.data(), GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::render(const glm::mat4& view, const glm::mat4& projection, const glm::vec2& atlasDim, GLuint atlasTexture, double time) {

    this->bufferData();

    // Render all entities to frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->screenFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->instancedShader->useShader();
    this->instancedShader->renderSetup(view, projection, atlasDim);

    glBindTexture(GL_TEXTURE_2D, atlasTexture);

    glBindVertexArray(this->VAO);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, this->textureCoordinatesBufferData.size()); 


    // Draw frame buffer to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    
    glClear(GL_COLOR_BUFFER_BIT);

    this->screenShader->useShader();
    this->screenShader->renderSetup(time);

    glBindTexture(GL_TEXTURE_2D, this->screenTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);   

    glUseProgram(0);
    glBindVertexArray(0);

    textureCoordinatesBufferData.clear();
    modelsBufferData.clear();
}