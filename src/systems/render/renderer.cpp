#include "renderer.hpp"

Renderer::Renderer() : screen_shader{ new ScreenShader() }, 
    instanced_shader{ new InstancedShader() } {

        glClearColor(0.0f, 0.4f, 0.4f, 0.0f);
        glEnable(GL_BLEND); 
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        this->initVAO();
        this->initScreenFBO();
        this->initVBOs();
}

Renderer::~Renderer() {
    glDeleteFramebuffers(1, &(this->screen_fbo));  
}

void Renderer::initVAO() {

    // create vao
    float quad_vertex_data[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &(this->vao));
    glBindVertexArray(this->vao);
    
    // The verticies will never change so the buffer ID is not saved
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_data), quad_vertex_data, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // Free bound buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
}

void Renderer::initVBOs() {

    glGenBuffers(1, &(this->texture_coordinates_vbo));
    glGenBuffers(1, &(this->models_vbo));

    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->texture_coordinates_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->texture_coordinates_buffer_data.size(), this->texture_coordinates_buffer_data.data(), GL_STREAM_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glVertexAttribDivisor(1, 1); 

    glBindBuffer(GL_ARRAY_BUFFER, this->models_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * this->models_buffer_data.size(), this->texture_coordinates_buffer_data.data(), GL_STREAM_DRAW);

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

    glGenFramebuffers(1, &(this->screen_fbo));
    glBindFramebuffer(GL_FRAMEBUFFER, this->screen_fbo);

    glGenTextures(1, &(this->screen_texture));
    glBindTexture(GL_TEXTURE_2D, this->screen_texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_c::SCREEN_WIDTH, render_c::SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->screen_texture, 0); 

    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void Renderer::addBufferData(const glm::vec4& texture_data, const glm::mat4& model_data) {

    this->texture_coordinates_buffer_data.push_back(texture_data);
    this->models_buffer_data.push_back(model_data);
}

void Renderer::bufferData() {

    if (this->maxBufferSize < this->models_buffer_data.size()) {

        this->maxBufferSize = this->models_buffer_data.size();

        glBindBuffer(GL_ARRAY_BUFFER, this->texture_coordinates_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*this->texture_coordinates_buffer_data.size(), this->texture_coordinates_buffer_data.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, this->models_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*this->models_buffer_data.size(), this->models_buffer_data.data(), GL_DYNAMIC_DRAW);
    
    } else {

        glBindBuffer(GL_ARRAY_BUFFER, this->texture_coordinates_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4)*this->texture_coordinates_buffer_data.size(), this->texture_coordinates_buffer_data.data());

        glBindBuffer(GL_ARRAY_BUFFER, this->models_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4)*this->models_buffer_data.size(), this->models_buffer_data.data());
    }

    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::render(const glm::mat4& view, const glm::mat4& projection, const glm::vec2& atlas_dimensions, GLuint atlas_texture, double time) {

    this->bufferData();

    // Render all entities to frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->screen_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->instanced_shader->useShader();
    this->instanced_shader->renderSetup(view, projection, atlas_dimensions);

    glBindTexture(GL_TEXTURE_2D, atlas_texture);

    glBindVertexArray(this->vao);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, this->texture_coordinates_buffer_data.size()); 


    // Draw frame buffer to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    
    glClear(GL_COLOR_BUFFER_BIT);

    this->screen_shader->useShader();
    this->screen_shader->renderSetup(time);

    glBindTexture(GL_TEXTURE_2D, this->screen_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);   

    glUseProgram(0);
    glBindVertexArray(0);

    texture_coordinates_buffer_data.clear();
    models_buffer_data.clear();
}