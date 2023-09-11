#include <vector>
#include <iostream>

#include "consts.hpp"
#include "shader_program.hpp"
#include "clock.hpp"
#include "camera.hpp"
#include "texture_atlas.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void clear();
    void queue(
        const glm::vec4& texture_data, 
        const glm::mat4& model_data,
        ShaderProgram* shader_program
    );
    void render();
    void present();


    GLuint getScreenTexture();
    void setPostProcessingShader(ShaderProgram* shader_program);

private:
    void initVAO();
    void initVBOs();
    void initScreenFBO();
    void initPixelPassFBO();
    void initFinalFBO();

    void bufferData(size_t start, size_t end);
    void renderPartialBuffer(size_t start, size_t end, ShaderProgram* shader_program);

    void renderPostProcessing();
    
    GLuint vao;
    GLuint screen_fbo;
    GLuint screen_texture;

    GLuint pixel_pass_fbo;
    GLuint pixel_pass_texture;

    GLuint final_fbo;
    GLuint final_texture;

    GLuint texture_coordinates_vbo;
    std::vector<glm::vec4> texture_coordinates_buffer_data; 

    GLuint models_vbo;
    std::vector<glm::mat4> models_buffer_data;

    std::vector<ShaderProgram*> shader_programs;
    ShaderProgram* post_processing_shader;

    size_t max_buffer_size{0}; 
};