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
    this->shaders["instanced_dialog_box"] = this->simpleInstancedShader(
        registry,
        "./src/systems/render/shaders/instanced_dialog_box/vertex.glsl",
        "./src/systems/render/shaders/instanced_dialog_box/fragment.glsl"
    );
}

ShaderProgram*& ShaderManager::operator[](const char* shader_name) {
    return this->shaders[shader_name];
}

ShaderProgram* ShaderManager::simpleInstancedShader(entt::registry& registry, const char* vertex_source, const char* fragment_source) {
    return new ShaderProgram(
        // {
        //     {UniformDataType::FLOAT, "time"},
        //     {UniformDataType::VEC_2, "atlas_dimensions"},
        //     {UniformDataType::VEC_2, "screen_resolution"},
        //     {UniformDataType::MAT_4X4, "V"},
        //     {UniformDataType::MAT_4X4, "P"},
        //     {UniformDataType::FLOAT, "camera_zoom"},
        //     {UniformDataType::TEX_2D, "atlas_texture"}
        // },
        vertex_source,
        fragment_source,
        this->logs
    );
}

ShaderProgram* ShaderManager::simpleScreenShader(entt::registry& registry, const char* vertex_source, const char* fragment_source) {
    return new ShaderProgram(
        // {
        //     {UniformDataType::FLOAT, "time"},
        //     {UniformDataType::VEC_2, "screen_resolution"},
        //     {UniformDataType::TEX_2D, "screen_texture"}
        // },
        vertex_source,
        fragment_source,
        this->logs
    );
}