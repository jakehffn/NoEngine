#include "shader_manager.hpp"

ShaderManager::ShaderManager(entt::registry& registry) {
    this->shaders["screen"] = this->simpleScreenShader(
        registry,
        "./src/systems/render/shaders/screen/vertex.glsl",
        "./src/systems/render/shaders/screen/fragment.glsl"
    );

    this->shaders["screen_blur"] = this->simpleScreenShader(
        registry,
        "./src/systems/render/shaders/screen_blur/vertex.glsl",
        "./src/systems/render/shaders/screen_blur/fragment.glsl"
    );

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
        [vertex_source, fragment_source, this](ShaderProgram* shader_program){
            auto id = LoadShaders(vertex_source, fragment_source, this->logs);

            shader_program->setUniformId("time", glGetUniformLocation(id, "time")); 
            shader_program->setUniformId("atlas_dimensions", glGetUniformLocation(id, "atlas_dimensions"));
            shader_program->setUniformId("screen_resolution", glGetUniformLocation(id, "screen_resolution"));

            shader_program->setUniformId("V", glGetUniformLocation(id, "V"));
            shader_program->setUniformId("P", glGetUniformLocation(id, "P"));
            shader_program->setUniformId("camera_zoom", glGetUniformLocation(id, "camera_zoom"));
        
            return id;
        }, 
        [&registry](ShaderProgram* shader_program, size_t num_verts, GLuint vao, GLuint dest_fbo, GLuint prev_texture){
            glBindFramebuffer(GL_FRAMEBUFFER, dest_fbo);
            shader_program->use();
            glBindVertexArray(vao);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            Clock& clock = registry.ctx().at<Clock&>();
            glUniform1f(shader_program->getUniformId("time"), clock.getCumulativeTime());
            TextureAtlas& texture_atlas = registry.ctx().at<TextureAtlas&>();
            glUniform2f(shader_program->getUniformId("atlas_dimensions"), texture_atlas.width, texture_atlas.height);
            glUniform2f(shader_program->getUniformId("screen_resolution"), globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT);
    
            glUniformMatrix4fv(shader_program->getUniformId("V"), 1, GL_FALSE, shader_program->getUniform("V"));
            glUniformMatrix4fv(shader_program->getUniformId("P"), 1, GL_FALSE, shader_program->getUniform("P"));
            glUniform1f(shader_program->getUniformId("camera_zoom"), *shader_program->getUniform("camera_zoom"));

            glBindTexture(GL_TEXTURE_2D, texture_atlas.gl_texture_id);

            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, num_verts); 
        }
    };
}

ShaderProgram* ShaderManager::simpleScreenShader(entt::registry& registry, const char* vertex_source, const char* fragment_source) {
    return new ShaderProgram {
        [vertex_source, fragment_source, this](ShaderProgram* shader_program){
            auto id = LoadShaders(vertex_source, fragment_source, this->logs);

            shader_program->setUniformId("time", glGetUniformLocation(id, "time")); 

            shader_program->setUniformId("screen_resolution", glGetUniformLocation(id, "screen_resolution"));

            return id;
        }, 
        [&registry](ShaderProgram* shader_program, size_t num_verts, GLuint vao, GLuint dest_fbo, GLuint prev_texture){
            glBindFramebuffer(GL_FRAMEBUFFER, dest_fbo);
            shader_program->use();

            Clock& clock = registry.ctx().at<Clock&>();
            glUniform1f(shader_program->getUniformId("time"), clock.getCumulativeTime());
    
            glUniform2f(shader_program->getUniformId("screen_resolution"), globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT);

            glBindVertexArray(vao);

            glBindTexture(GL_TEXTURE_2D, prev_texture);
            glDrawArrays(GL_TRIANGLES, 0, num_verts);   
        }
    };
}