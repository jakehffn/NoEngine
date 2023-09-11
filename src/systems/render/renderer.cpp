#include "renderer.hpp"

Renderer::Renderer() {
        glClearColor(0.0f, 0.4f, 0.4f, 0.0f);
        glEnable(GL_BLEND); 
        glEnable(GL_STENCIL_TEST);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        this->initVAO();
        this->initScreenFBO();
        this->initPixelPassFBO();
        this->initFinalFBO();
        this->initVBOs();
}

Renderer::~Renderer() {
    glDeleteFramebuffers(1, &(this->screen_fbo));  
}

GLuint Renderer::getScreenTexture() {
    return this->final_texture;
}

void Renderer::setPostProcessingShader(ShaderProgram* shader_program) {
    this->post_processing_shader = shader_program;
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
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glGenTextures(1, &(this->screen_texture));
    glBindTexture(GL_TEXTURE_2D, this->screen_texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, constant::SCREEN_WIDTH, constant::SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->screen_texture, 0); 
    
    GLuint depth_stencil_buffer;
    glGenRenderbuffers(1, &depth_stencil_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, constant::SCREEN_WIDTH, constant::SCREEN_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // attach render buffer to the fbo as depth buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_buffer);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void Renderer::initPixelPassFBO() {
    glGenFramebuffers(1, &(this->pixel_pass_fbo));
    glBindFramebuffer(GL_FRAMEBUFFER, this->pixel_pass_fbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glGenTextures(1, &(this->pixel_pass_texture));
    glBindTexture(GL_TEXTURE_2D, this->pixel_pass_texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, constant::SCREEN_WIDTH / 5, constant::SCREEN_HEIGHT / 5, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->pixel_pass_texture, 0); 
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void Renderer::initFinalFBO() {
    glGenFramebuffers(1, &(this->final_fbo));
    glBindFramebuffer(GL_FRAMEBUFFER, this->final_fbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glGenTextures(1, &(this->final_texture));
    glBindTexture(GL_TEXTURE_2D, this->final_texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, constant::SCREEN_WIDTH, constant::SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->final_texture, 0); 
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void Renderer::bufferData(size_t start, size_t end) {
    const auto buffer_data_size = end - start;
    if (this->max_buffer_size < buffer_data_size) {
        this->max_buffer_size = buffer_data_size;

        glBindBuffer(GL_ARRAY_BUFFER, this->texture_coordinates_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*buffer_data_size, &(this->texture_coordinates_buffer_data[start]), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, this->models_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*buffer_data_size, &(this->models_buffer_data[start]), GL_DYNAMIC_DRAW);
    
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, this->texture_coordinates_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4)*buffer_data_size, &(this->texture_coordinates_buffer_data[start]));

        glBindBuffer(GL_ARRAY_BUFFER, this->models_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4)*buffer_data_size, &(this->models_buffer_data[start]));
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::clear() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->screen_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::queue(const glm::vec4& texture_data, const glm::mat4& model_data, ShaderProgram* shader_program) {
    this->texture_coordinates_buffer_data.push_back(texture_data);
    this->models_buffer_data.push_back(model_data);
    this->shader_programs.push_back(shader_program);
}

void Renderer::render() {
    size_t start = 0, end = 0;
    ShaderProgram* last_shader{this->shader_programs[0]};
    ShaderProgram* next_shader;
    while (++end < this->shader_programs.size()) {
        next_shader = this->shader_programs[end];
        if (this->shader_programs[end] != last_shader) {
            this->renderPartialBuffer(start, end, last_shader);
            start = end;
        }
        last_shader = next_shader;
    }
    this->renderPartialBuffer(start, this->models_buffer_data.size(), last_shader);
    
    this->texture_coordinates_buffer_data.clear();
    this->models_buffer_data.clear();
    this->shader_programs.clear();
}

void Renderer::present() {
    this->renderPostProcessing();
}

void Renderer::renderPartialBuffer(size_t start, size_t end, ShaderProgram* shader_program) {
    this->bufferData(start, end);
    shader_program->render(end - start, this->vao, this->screen_fbo, this->screen_texture);
}

void Renderer::renderPostProcessing() {
    // Render to final texture
    glBindFramebuffer(GL_FRAMEBUFFER, this->final_fbo);
    glClear(GL_COLOR_BUFFER_BIT);

    this->post_processing_shader->render(6, this->vao, this->final_fbo, this->screen_texture);
    
    // Render to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    this->post_processing_shader->render(6, this->vao, 0, this->final_texture);

    glUseProgram(0);
    glBindVertexArray(0);
}