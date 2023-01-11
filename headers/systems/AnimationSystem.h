#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "System.h"
#include "Clock.h"

#include "Texture.h"
#include "Animation.h"
#include "IdleAnimation.h"
#include "MoveAnimation.h"

#include "Spacial.h"
#include "Velocity.h"

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