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

    this->shaders["instanced"] = new ShaderProgram {
        [](ShaderProgram* shader_program){
            std::string vertex_shader_path = "./src/systems/render/shaders/instanced/vertex.glsl";
            std::string fragment_shader_path = "./src/systems/render/shaders/instanced/fragment.glsl";

            auto id = LoadShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());

            shader_program->setUniformId("V", glGetUniformLocation(id, "V"));
            shader_program->setUniformId("P", glGetUniformLocation(id, "P"));
            shader_program->setUniformId("atlas_dimensions", glGetUniformLocation(id, "atlas_dimensions"));
        
            return id;
        }, 
        [&registry](ShaderProgram* shader_program, size_t num_verts, GLuint vao, GLuint dest_fbo, GLuint prev_texture){
            using namespace entt::literals;

            Camera& camera = registry.ctx().at<Camera&>("world_camera"_hs);
            TextureAtlas& texture_atlas = registry.ctx().at<TextureAtlas&>();
            Clock& clock = registry.ctx().at<Clock&>();

            glBindFramebuffer(GL_FRAMEBUFFER, dest_fbo);
            shader_program->use();
            glBindVertexArray(vao);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
            glUniformMatrix4fv(shader_program->getUniformId("V"), 1, GL_FALSE, &(camera.getViewMatrix()[0][0]));
            glUniformMatrix4fv(shader_program->getUniformId("P"), 1, GL_FALSE, &(camera.getProjectionMatrix())[0][0]);
            glUniform2f(shader_program->getUniformId("atlas_dimensions"), texture_atlas.width, texture_atlas.height);

            glBindTexture(GL_TEXTURE_2D, texture_atlas.gl_texture_id);

            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, num_verts); 
        }
    };

    this->shaders["other_instanced"] = new ShaderProgram {
        [](ShaderProgram* shader_program){
            std::string vertex_shader_path = "./src/systems/render/shaders/instanced_other/vertex.glsl";
            std::string fragment_shader_path = "./src/systems/render/shaders/instanced_other/fragment.glsl";

            auto id = LoadShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());

            shader_program->setUniformId("V", glGetUniformLocation(id, "V"));
            shader_program->setUniformId("P", glGetUniformLocation(id, "P"));
            shader_program->setUniformId("atlas_dimensions", glGetUniformLocation(id, "atlas_dimensions"));
        
            return id;
        }, 
        [&registry](ShaderProgram* shader_program, size_t num_verts, GLuint vao, GLuint dest_fbo, GLuint prev_texture){
            using namespace entt::literals;

            Camera& camera = registry.ctx().at<Camera&>("world_camera"_hs);
            TextureAtlas& texture_atlas = registry.ctx().at<TextureAtlas&>();
            Clock& clock = registry.ctx().at<Clock&>();

            glBindFramebuffer(GL_FRAMEBUFFER, dest_fbo);
            shader_program->use();
            glBindVertexArray(vao);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
            glUniformMatrix4fv(shader_program->getUniformId("V"), 1, GL_FALSE, &(camera.getViewMatrix()[0][0]));
            glUniformMatrix4fv(shader_program->getUniformId("P"), 1, GL_FALSE, &(camera.getProjectionMatrix())[0][0]);
            glUniform2f(shader_program->getUniformId("atlas_dimensions"), texture_atlas.width, texture_atlas.height);

            glBindTexture(GL_TEXTURE_2D, texture_atlas.gl_texture_id);

            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, num_verts); 
        }
    };

    this->shaders["instanced_outline"] = new ShaderProgram {
        [](ShaderProgram* shader_program){
            std::string vertex_shader_path = "./src/systems/render/shaders/sprite_outline/vertex.glsl";
            std::string fragment_shader_path = "./src/systems/render/shaders/sprite_outline/fragment.glsl";

            auto id = LoadShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());

            shader_program->setUniformId("V", glGetUniformLocation(id, "V"));
            shader_program->setUniformId("P", glGetUniformLocation(id, "P"));
            shader_program->setUniformId("atlas_dimensions", glGetUniformLocation(id, "atlas_dimensions"));
        
            return id;
        }, 
        [&registry](ShaderProgram* shader_program, size_t num_verts, GLuint vao, GLuint dest_fbo, GLuint prev_texture){
            using namespace entt::literals;

            Camera& camera = registry.ctx().at<Camera&>("world_camera"_hs);
            TextureAtlas& texture_atlas = registry.ctx().at<TextureAtlas&>();
            Clock& clock = registry.ctx().at<Clock&>();

            glBindFramebuffer(GL_FRAMEBUFFER, dest_fbo);
            shader_program->use();
            glBindVertexArray(vao);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
            glUniformMatrix4fv(shader_program->getUniformId("V"), 1, GL_FALSE, &(camera.getViewMatrix()[0][0]));
            glUniformMatrix4fv(shader_program->getUniformId("P"), 1, GL_FALSE, &(camera.getProjectionMatrix())[0][0]);
            glUniform2f(shader_program->getUniformId("atlas_dimensions"), texture_atlas.width, texture_atlas.height);

            glBindTexture(GL_TEXTURE_2D, texture_atlas.gl_texture_id);

            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, num_verts); 
        }
    };
}

ShaderProgram*& ShaderManager::operator[](const char* shader_name) {
    return this->shaders[shader_name];
}