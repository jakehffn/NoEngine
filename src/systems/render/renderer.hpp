#include <vector>
#include <iostream>

#include "globals.hpp"
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

    void renderPostProcessing(ShaderProgram* shader_program);
    void present(ShaderProgram* shader_program);

    GLuint getScreenTexture();

private:
    void initVAO();
    void initVBOs();
    void initScreenFBOs();
    void initPixelPassFBO();
    void initFinalFBO();

    void bufferData(size_t start, size_t end);
    void renderPartialBuffer(size_t start, size_t end, ShaderProgram* shader_program);

    
    GLuint vao;
    GLuint current_screen_fbo;
    GLuint other_screen_fbo;
    GLuint current_screen_texture;
    GLuint other_screen_texture;

    GLuint texture_coordinates_vbo;
    std::vector<glm::vec4> texture_coordinates_buffer_data; 

    GLuint models_vbo;
    std::vector<glm::mat4> models_buffer_data;

    std::vector<ShaderProgram*> shader_programs;

    size_t max_buffer_size{0}; 
};