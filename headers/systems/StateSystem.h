#pragma once

#include <entt/entt.hpp>

#include "System.h"

#include "SpriteState.h"
#include "Sprite.h"

class StateSystem : public System {
public:
    StateSystem(entt::registry& registry);

    void update(entt::registry& registry);
    void systemState() override;

private:
    void updateSpriteState(SpriteState state, Sprite& sprite);

    entt::observer spriteStateObserver;
};