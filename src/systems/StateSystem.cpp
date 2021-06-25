#include "StateSystem.h"

StateSystem::StateSystem(entt::registry& registry) : 
    spriteStateObserver{ entt::observer(registry, entt::collector.update<SpriteState>()) } {}

void StateSystem::update(entt::registry& registry) {

    for (const auto entity : spriteStateObserver) {

        auto [spriteState, sprite, animation] = registry.get<SpriteState, Sprite, Animation>(entity);

        auto [newSprite, newAnimation] = spriteState.stateMap[spriteState.state];

        sprite = newSprite;
        animation = newAnimation;
    }

    spriteStateObserver.clear();
}

void StateSystem::systemState() {}