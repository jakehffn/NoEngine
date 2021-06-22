#include "RenderSystem.h"

RenderSystem::RenderSystem(entt::registry& registry) : shaderProgram{ new BasicShader() } {
    // initialize group with empty registry for performance
    auto init = registry.group<Sprite>(entt::get<Model>);
}

void RenderSystem::update(entt::registry& registry) {

    auto sprites = registry.group<Sprite>(entt::get<Model>);

    for (auto entity : sprites) {

        auto [sprite, model] = sprites.get(entity);

        renderSprite(sprite, model);
    }
}