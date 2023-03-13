#include "instanced_shader.hpp"

InstancedShader::InstancedShader() {
    
    std::string vertex_shader_path = "./src/systems/render/shaders/instanced/src/vertexShader.glsl";
    std::string fragment_shader_path = "./src/systems/render/shaders/instanced/src/fragmentShader.glsl";

    this->gl_shader_program_id = LoadShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());

    this->v_uniform_id = glGetUniformLocation(this->gl_shader_program_id, "V");
    this->p_uniform_id = glGetUniformLocation(this->gl_shader_program_id, "P");

    this->atlas_dimensions_uniform_id = glGetUniformLocation(this->gl_shader_program_id, "atlasDimensions");
}
void InstancedShader::renderSetup(glm::mat4 view, glm::mat4 projection, glm::vec2 atlas_dimensions) {

    glUniformMatrix4fv(this->v_uniform_id, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(this->p_uniform_id, 1, GL_FALSE, &projection[0][0]);
    glUniform2f(this->atlas_dimensions_uniform_id, atlas_dimensions.x, atlas_dimensions.y);
}