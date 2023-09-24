#pragma once

#include <filesystem>

#include <entt/entt.hpp>

#include "name.hpp"
#include "spacial.hpp"
#include "renderable.hpp"
#include "component_grid_ignore.hpp"
#include "gui_element.hpp"
#include "fps_counter.hpp"
#include "text.hpp"

static void FpsCounterEntity(entt::registry& registry, entt::entity entity) {
    std::string name = "FPS Counter";

    registry.emplace<Name>(entity, name);
    registry.emplace<Spacial>(entity, glm::vec3(100, 100, 0));
    registry.emplace<GuiElement>(entity);
    registry.emplace<FpsCounter>(entity);
    registry.emplace<Text>(entity, U"POGGERS POGGERS POGGERS");
}