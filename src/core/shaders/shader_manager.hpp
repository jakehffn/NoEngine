#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include <entt/entt.hpp>

#include "globals.hpp"
#include "shader_program.hpp"
#include "clock.hpp"
#include "camera.hpp"
#include "texture_atlas.hpp"
#include "uniform.hpp"

struct ShaderManager {
    ShaderManager(entt::registry& registry);
    ShaderProgram* simpleInstancedShader(entt::registry& registry, const char* vertex_source, const char* fragment_source);
    ShaderProgram* simpleScreenShader(entt::registry& registry, const char* vertex_source, const char* fragment_source);
    template <typename T>
    void setAllUniforms(const char* uniform_name, T uniform_data) {
        for (auto [name, shader] : this->shaders) {
            shader->setUniform(uniform_name, uniform_data);
        }
    }
    ShaderProgram*& operator[](const char* shader_name);
    std::unordered_map<std::string, ShaderProgram*> shaders;
    std::vector<std::string> logs;
};