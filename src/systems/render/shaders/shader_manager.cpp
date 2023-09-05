#include "shader_manager.hpp"

ShaderManager::ShaderManager(entt::registry& registry) {
    this->shaders["screen"] = new ShaderProgram {
        [](ShaderProgram* shader_program){
            std::string vertex_shader_path = "./src/systems/render/shaders/screen/vertexShader.glsl";
            std::string fragment_shader_path = "./src/systems/render/shaders/screen/fragmentShader.glsl";

            auto id = LoadShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());
            shader_program->setUniform("time", glGetUniformLocation(id, "time")); 

            return id;
        }, 
        [&registry](ShaderProgram* shader_program){
            Clock& clock = registry.ctx().at<Clock&>();
            glUniform1f(shader_program->getUniform("time"), clock.getCumulativeTime());
        }
    };

    this->shaders["instanced"] = new ShaderProgram {
        [](ShaderProgram* shader_program){
            std::string vertex_shader_path = "./src/systems/render/shaders/instanced/vertexShader.glsl";
            std::string fragment_shader_path = "./src/systems/render/shaders/instanced/fragmentShader.glsl";

            auto id = LoadShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());

            shader_program->setUniform("V", glGetUniformLocation(id, "V"));
            shader_program->setUniform("P", glGetUniformLocation(id, "P"));
            shader_program->setUniform("atlasDimensions", glGetUniformLocation(id, "atlasDimensions"));
        
            return id;
        }, 
        [&registry](ShaderProgram* shader_program){
            using namespace entt::literals;

            Camera& camera = registry.ctx().at<Camera&>("world_camera"_hs);
            TextureAtlas& texture_atlas = registry.ctx().at<TextureAtlas&>();
            Clock& clock = registry.ctx().at<Clock&>();


            glUniformMatrix4fv(shader_program->getUniform("V"), 1, GL_FALSE, &(camera.getViewMatrix()[0][0]));
            glUniformMatrix4fv(shader_program->getUniform("P"), 1, GL_FALSE, &(camera.getProjectionMatrix())[0][0]);
            glUniform2f(shader_program->getUniform("atlasDimensions"), texture_atlas.width, texture_atlas.height);

            glBindTexture(GL_TEXTURE_2D, texture_atlas.gl_texture_id);
        }
    };

    this->shaders["other_instanced"] = new ShaderProgram {
        [](ShaderProgram* shader_program){
            std::string vertex_shader_path = "./src/systems/render/shaders/instanced_other/vertexShader.glsl";
            std::string fragment_shader_path = "./src/systems/render/shaders/instanced_other/fragmentShader.glsl";

            auto id = LoadShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());

            shader_program->setUniform("V", glGetUniformLocation(id, "V"));
            shader_program->setUniform("P", glGetUniformLocation(id, "P"));
            shader_program->setUniform("atlasDimensions", glGetUniformLocation(id, "atlasDimensions"));
        
            return id;
        }, 
        [&registry](ShaderProgram* shader_program){
            using namespace entt::literals;

            Camera& camera = registry.ctx().at<Camera&>("world_camera"_hs);
            TextureAtlas& texture_atlas = registry.ctx().at<TextureAtlas&>();
            Clock& clock = registry.ctx().at<Clock&>();


            glUniformMatrix4fv(shader_program->getUniform("V"), 1, GL_FALSE, &(camera.getViewMatrix()[0][0]));
            glUniformMatrix4fv(shader_program->getUniform("P"), 1, GL_FALSE, &(camera.getProjectionMatrix())[0][0]);
            glUniform2f(shader_program->getUniform("atlasDimensions"), texture_atlas.width, texture_atlas.height);

            glBindTexture(GL_TEXTURE_2D, texture_atlas.gl_texture_id);
        }
    };
}

ShaderProgram*& ShaderManager::operator[](const char* shader_name) {
    return this->shaders[shader_name];
}