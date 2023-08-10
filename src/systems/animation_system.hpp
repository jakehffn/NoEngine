#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "system.hpp"
#include "clock.hpp"

#include "texture.hpp"
#include "animation.hpp"
#include "animator.hpp"
#include "idle_animation.hpp"
#include "move_animation.hpp"

#include "spacial.hpp"
#include "velocity.hpp"

class AnimationSystem : public System {
public: 
    AnimationSystem(entt::registry& registry);

    void update() override;

private:
    void updateAnimators();
    void updateTextures();

    void updateIdleAnimations();
    void updateMoveAnimations();

    entt::observer idle_animation_observer;
    entt::observer move_animation_observer;
};