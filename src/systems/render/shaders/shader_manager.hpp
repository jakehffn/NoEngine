#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include <entt/entt.hpp>

#include "shader_program.hpp"
#include "clock.hpp"
#include "camera.hpp"
#include "texture_atlas.hpp"

struct ShaderManager {
    ShaderManager(entt::registry& registry);
    ShaderProgram*& operator[](const char* shader_name);
    std::unordered_map<std::string, ShaderProgram*> shaders;
};