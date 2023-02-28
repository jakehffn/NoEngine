#include <vector>

#include "consts.h"
#include "shader_program.h"
#include "sprite_shader.h"
#include "tile_shader.h"
#include "screen_shader.h"
#include "instanced_shader.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void addBufferData(glm::vec4 textureData, glm::mat4 modelData);
    void render(glm::mat4 view, glm::mat4 projection, glm::vec2 atlasDim, GLuint atlasTexture, double time);

private:
    void initVAO();
    void initVBOs();
    void initScreenFBO();

    void bufferData();

    GLuint VAO;

    GLuint screenFBO;
    GLuint screenTexture;

    GLuint textureCoordinatesVBO;
    std::vector<glm::vec4> textureCoordinatesBufferData; 

    GLuint modelsVBO;
    std::vector<glm::mat4> modelsBufferData; 

    InstancedShader* instancedShader;
    ShaderProgram<double>* screenShader;
};