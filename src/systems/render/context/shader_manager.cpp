#include "shader_manager.hpp"

ShaderManager::ShaderManager(entt::registry& registry) {
    this->shaders["screen"] = new ShaderProgram {
        [](ShaderProgram* shader_program){
            std::string vertex_shader_path = "./src/systems/render/shaders/screen/vertex.glsl";
            std::string fragment_shader_path = "./src/systems/render/shaders/screen/fragment.glsl";

            auto id = LoadShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());
            shader_program->setUniformId("time", glGetUniformLocation(id, "time")); 

            return id;
        }, 
        [&registry](ShaderProgram* shader_program, size_t num_verts, GLuint vao, GLuint dest_fbo, GLuint prev_texture){
            Clock& clock = registry.ctx().at<Clock&>();

            glBindFramebuffer(GL_FRAMEBUFFER, dest_fbo);
            shader_program->use();
            glUniform1f(shader_program->getUniformId("time"), clock.getCumulativeTime());

            glBindVertexArray(vao);

            glBindTexture(GL_TEXTURE_2D, prev_texture);
            glDrawArrays(GL_TRIANGLES, 0, num_verts);   
        }
    };

    this->shaders["instanced"] = this->simpleInstancedShader(
        registry,
        "./src/systems/render/shaders/instanced/vertex.glsl",
        "./src/systems/render/shaders/instanced/fragment.glsl"
    );

    this->shaders["instanced_other"] = this->simpleInstancedShader(
        registry,
        "./src/systems/render/shaders/instanced_other/vertex.glsl",
        "./src/systems/render/shaders/instanced_other/fragment.glsl"
    );

    this->shaders["instanced_outline"] = this->simpleInstancedShader(
        registry,
        "./src/systems/render/shaders/instanced_outline/vertex.glsl",
        "./src/systems/render/shaders/instanced_outline/fragment.glsl"
    );

    this->shaders["instanced_sharp_outline"] = this->simpleInstancedShader(
        registry,
        "./src/systems/render/shaders/instanced_outline_sharp/vertex.glsl",
        "./src/systems/render/shaders/instanced_outline_sharp/fragment.glsl"
    );

    this->shaders["instanced_backdrop"] = this->simpleInstancedShader(
        registry,
        "./src/systems/render/shaders/instanced_drop_shadow/vertex.glsl",
        "./src/systems/render/shaders/instanced_drop_shadow/fragment.glsl"
    );

    this->shaders["instanced_inline"] = this->simpleInstancedShader(
        registry,
        "./src/systems/render/shaders/instanced_inline/vertex.glsl",
        "./src/systems/render/shaders/instanced_inline/fragment.glsl"
    );
}

ShaderProgram*& ShaderManager::operator[](const char* shader_name) {
    return this->shaders[shader_name];
}

ShaderProgram* ShaderManager::simpleInstancedShader(entt::registry& registry, const char* vertex_source, const char* fragment_source) {
    return new ShaderProgram {
        [vertex_source, fragment_source](ShaderProgram* shader_program){
            auto id = LoadShaders(vertex_source, fragment_source);

            shader_program->setUniformId("V", glGetUniformLocation(id, "V"));
            shader_program->setUniformId("P", glGetUniformLocation(id, "P"));
            shader_program->setUniformId("atlas_dimensions", glGetUniformLocation(id, "atlas_dimensions"));
        
            return id;
        }, 
        [&registry](ShaderProgram* shader_program, size_t num_verts, GLuint vao, GLuint dest_fbo, GLuint prev_texture){
            TextureAtlas& texture_atlas = registry.ctx().at<TextureAtlas&>();
            Clock& clock = registry.ctx().at<Clock&>();

            glBindFramebuffer(GL_FRAMEBUFFER, dest_fbo);
            shader_program->use();
            glBindVertexArray(vao);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
            glUniformMatrix4fv(shader_program->getUniformId("V"), 1, GL_FALSE, shader_program->getUniform("V"));
            glUniformMatrix4fv(shader_program->getUniformId("P"), 1, GL_FALSE, shader_program->getUniform("P"));
            glUniform2f(shader_program->getUniformId("atlas_dimensions"), texture_atlas.width, texture_atlas.height);

            glBindTexture(GL_TEXTURE_2D, texture_atlas.gl_texture_id);

            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, num_verts); 
        }
    };
}