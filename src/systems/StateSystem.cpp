#include "StateSystem.h"

StateSystem::StateSystem(entt::registry& registry) : 
    spriteStateObserver{ entt::observer(registry, entt::collector.update<SpriteState>()) } {}

void StateSystem::update(entt::registry& registry) {

    for (const auto entity : spriteStateObserver) {

        auto [spriteState, sprite] = registry.get<SpriteState, Sprite>(entity);

        updateSpriteState(spriteState, sprite);
    }
}

void StateSystem::updateSpriteState(SpriteState state, Sprite& sprite) {
    sprite = state.stateMap[state.state];
}

void StateSystem::systemState() {}