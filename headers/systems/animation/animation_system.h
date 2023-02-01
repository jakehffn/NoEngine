#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "system.h"
#include "clock.h"

#include "texture.h"
#include "animation.h"
#include "idle_animation.h"
#include "move_animation.h"

#include "spacial.h"
#include "velocity.h"

class AnimationSystem : public System {
public: 
    AnimationSystem(entt::registry& registry);

    void update() override;

private:

    void updateIdleAnimations();
    void updateMoveAnimations();

    void updateAnimations();

    entt::observer idleAnimationObserver;
    entt::observer moveAnimationObserver;
};